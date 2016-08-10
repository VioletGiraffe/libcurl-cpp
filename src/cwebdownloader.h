#pragma once

#include "compiler/compiler_warnings_control.h"
#include "export.h"

DISABLE_COMPILER_WARNINGS
#include <QString>
RESTORE_COMPILER_WARNINGS

typedef void CURL;

class LIBCURL_CPP_EXPORT CWebDownloader
{
public:
	// This class is managing some resources and the instance is meant to be kept around if you need to download multiple URLs (as opposed to creating a new instance each time).
	explicit CWebDownloader(const QString& userAgent = "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_10_1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/41.0.2227.1 Safari/537.36");
	~CWebDownloader();

	void setUserAgent(const QString& userAgent);

	QByteArray download(const QString& url);

private:
	CURL* _curlHandle = nullptr;
};
