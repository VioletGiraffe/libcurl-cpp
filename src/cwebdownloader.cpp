#include "cwebdownloader.h"

#include "curl/curl.h"

DISABLE_COMPILER_WARNINGS
#include <QDebug>
RESTORE_COMPILER_WARNINGS

struct MemoryStruct {
	QByteArray dataBuffer;
	size_t bytesUsed = 0;
};

size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
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

CWebDownloader::CWebDownloader()
{

}

QByteArray CWebDownloader::download(const QString &url)
{
	CURL *curl_handle;
	CURLcode res;

	MemoryStruct chunk;
	chunk.dataBuffer.resize(16384); // Meaningful starting size of 16K

	curl_global_init(CURL_GLOBAL_ALL);

	/* init the curl session */
	curl_handle = curl_easy_init();

	/* specify URL to get */
	curl_easy_setopt(curl_handle, CURLOPT_URL, url.toUtf8().constData());

	/* send all data to this function  */
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

	/* we pass our 'chunk' struct to the callback function */
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *) &chunk);

	/* some servers don't like requests that are made without a user-agent
	   field, so we provide one */
	curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

	/* get it! */
	res = curl_easy_perform(curl_handle);

	/* check for errors */
	if (res != CURLE_OK)
		qDebug() << "curl_easy_perform() failed:" << curl_easy_strerror(res);
	else
		chunk.dataBuffer.truncate((int) chunk.bytesUsed);

	/* cleanup curl stuff */
	curl_easy_cleanup(curl_handle);

	/* we're done with libcurl, so clean it up */
	curl_global_cleanup();

	return res == CURLE_OK ? chunk.dataBuffer : QByteArray();
}
