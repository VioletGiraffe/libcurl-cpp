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
	explicit CWebDownloader(const QString& userAgent = "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:58.0) Gecko/20100101 Firefox/58.0");
	~CWebDownloader();

	void setUserAgent(const QString& userAgent);

	QByteArray download(const QString& url);

private:
	CURL* _curlHandle = nullptr;
};
