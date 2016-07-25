#pragma once

#include "compiler/compiler_warnings_control.h"
#include "export.h"

DISABLE_COMPILER_WARNINGS
#include <QString>
RESTORE_COMPILER_WARNINGS

class LIBCURL_CPP_EXPORT CWebDownloader
{
public:
	CWebDownloader();

	QByteArray download(const QString& url);
};
