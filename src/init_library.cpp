#include "assert/advanced_assert.h"

DISABLE_COMPILER_WARNINGS
#include <QDebug>
RESTORE_COMPILER_WARNINGS

struct StaticInitializer {
	StaticInitializer() {
		AdvancedAssert::setLoggingFunc([](const char* message){
			qDebug() << message;
		});
	}
} initializer;
