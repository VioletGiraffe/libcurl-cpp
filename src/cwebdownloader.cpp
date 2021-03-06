#include "cwebdownloader.h"
#include "assert/advanced_assert.h"

#include "curl/curl.h"

DISABLE_COMPILER_WARNINGS
#include <QDebug>
#include <QString>
RESTORE_COMPILER_WARNINGS

struct MemoryStruct {
	QByteArray dataBuffer;
	size_t bytesUsed = 0;
};

inline size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	const size_t newChunkSize = size * nmemb;
	MemoryStruct* mem = (MemoryStruct*) userp;

	const size_t oldBufferSize = mem->dataBuffer.size();
	const size_t requestedSize = mem->bytesUsed + newChunkSize;
	if (oldBufferSize < requestedSize)
		mem->dataBuffer.resize((int) std::max(requestedSize, oldBufferSize * 3 / 2)); // Increasing the size 1.5x times to reduce the number of allocations for scenarios with many chunks

	memcpy(mem->dataBuffer.data() + mem->bytesUsed, contents, newChunkSize);
	mem->bytesUsed += newChunkSize;

	return newChunkSize;
}

CWebDownloader::CWebDownloader(const QString& userAgent)
{
	curl_global_init(CURL_GLOBAL_ALL);

	_curlHandle = curl_easy_init();

	// send all data to this function
	assert_r(curl_easy_setopt(_curlHandle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback) == CURLE_OK);

	// follow redirects up to the specified number of hops
	assert_r(curl_easy_setopt(_curlHandle, CURLOPT_FOLLOWLOCATION, 30L) == CURLE_OK);

	// disable SSL certificate checking
	assert_r(curl_easy_setopt(_curlHandle, CURLOPT_SSL_VERIFYPEER, 0) == CURLE_OK);
	assert_r(curl_easy_setopt(_curlHandle, CURLOPT_SSL_VERIFYHOST, 0) == CURLE_OK);

	/* some servers don't like requests that are made without a user-agent field, so we provide one */
	assert_r(curl_easy_setopt(_curlHandle, CURLOPT_USERAGENT, userAgent.toUtf8().constData()) == CURLE_OK);
}

CWebDownloader::CWebDownloader() : CWebDownloader(QStringLiteral("Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:58.0) Gecko/20100101 Firefox/58.0"))
{
}

CWebDownloader::~CWebDownloader()
{
	/* cleanup curl stuff */
	curl_easy_cleanup(_curlHandle);

	/* we're done with libcurl, so clean it up */
	curl_global_cleanup();
}

QByteArray CWebDownloader::download(const QString &url)
{
	MemoryStruct chunk;
	chunk.dataBuffer.resize(16384); // Meaningful starting size of 16K

	// specify URL to get
	curl_easy_setopt(_curlHandle, CURLOPT_URL, url.toUtf8().constData());

	// we pass our 'chunk' struct to the callback function
	curl_easy_setopt(_curlHandle, CURLOPT_WRITEDATA, (void *) &chunk);

	// get it!
	const CURLcode res = curl_easy_perform(_curlHandle);

	// check for errors
	if (res != CURLE_OK)
	{
		assert_unconditional_r((QStringLiteral("curl_easy_perform() failed: ") + curl_easy_strerror(res)).toUtf8().constData());
	}
	else
		chunk.dataBuffer.truncate((int) chunk.bytesUsed);

	return res == CURLE_OK ? chunk.dataBuffer : QByteArray();
}
