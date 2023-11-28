#
# This PRI file takes only the classes needed from the
# 'default' port.
#

INCLUDEPATH += $$PWD/..

# Path to default implementation of the XF port
DEFAULT_IMPL_PATH = $$PWD/../default

DEFAULT_IMPL_SOURCES += \
    "$${DEFAULT_IMPL_PATH}/xf-default.cpp" \
    "$${DEFAULT_IMPL_PATH}/timeoutmanager-default.cpp" \
    "$${DEFAULT_IMPL_PATH}/dispatcher-active.cpp"

DEFAULT_IMPL_HEADERS += \
    "$${DEFAULT_IMPL_PATH}/timeoutmanager-default.h" \
    "$${DEFAULT_IMPL_PATH}/dispatcher-active.h"

SOURCES += \
    $${DEFAULT_IMPL_SOURCES}$$

HEADERS += \
    $${DEFAULT_IMPL_HEADERS}$$
