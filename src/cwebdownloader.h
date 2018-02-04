#pragma once

#include "compiler/compiler_warnings_control.h"
#include "export.h"

class QString;
class QByteArray;

typedef void CURL;

// This class is managing some resources and the instance is meant to be kept around if you need to download multiple URLs (as opposed to creating a new instance each time).
class LIBCURL_CPP_EXPORT CWebDownloader
{
public:	
	CWebDownloader();
	explicit CWebDownloader(const QString& userAgent);

	~CWebDownloader();

	void setUserAgent(const QString& userAgent);

	QByteArray download(const QString& url);

private:
	CURL* _curlHandle = nullptr;
};
