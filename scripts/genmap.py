#!/usr/bin/python2

import sys, os, re

components = {
    'core': 'Q_CORE_EXPORT',
    'gui': 'Q_GUI_EXPORT',
    'dbus': 'Q_DBUS_EXPORT',
    'declarative': 'Q_DECLARATIVE_EXPORT',
    'designer': 'QDESIGNER_COMPONENTS_EXPORT|QDESIGNER_EXTENSION_EXPORT|QT_FORMEDITOR_EXPORT|QT_PROPERTYEDITOR_EXPORT|QT_SIGNALSLOTEDITOR_EXPORT|QT_OBJECTINSPECTOR_EXPORT|QT_WIDGETBOX_EXPORT|QT_BUDDYEDITOR_EXPORT|QT_TABORDEREDITOR_EXPORT|QT_TASKMENU_EXPORT',
    'help': 'QHELP_EXPORT',
    'multimedia': 'Q_MULTIMEDIA_EXPORT',
    'network': 'Q_NETWORK_EXPORT',
    'sql': 'Q_SQL_EXPORT',
    'svg': 'Q_SVG_EXPORT',
    'xml': 'Q_XML_EXPORT',
    'xmlpatterns': 'Q_XMLPATTERNS_EXPORT',
    'script': 'Q_SCRIPT_EXPORT',
    'scripttools': 'Q_SCRIPTTOOLS_EXPORT',
    'test': 'Q_TESTLIB_EXPORT',
    'uitools': 'QDESIGNER_UILIB_EXPORT',
    'opengl': 'Q_OPENGL_EXPORT',
}
mapoutput = 'src/shared/qclass_lib_map.h'
classcount = -1
mapdata = '// Automatically generated by genmap.py, DO NOT EDIT!\n\n'
mapdata += '#ifndef QT_CLASS_MAP_H\n#define QT_CLASS_MAP_H\n\n'
mapdata += 'static const ClassInfoEntry qclass_lib_map[] = {\n'

def exportscan(sdir, keyword, component):
    dirmap = ''
    global classcount
    for sroot, sdir, lfiles in os.walk(sdir):
        for sfile in lfiles:
            if not sfile.endswith('.h'):
                continue
            sfull = '%s/%s' % (sroot, sfile)
            with open(sfull, 'rb') as f:
                scontent = f.read()
            scontent = scontent.decode('utf-8')
            for match in re.findall('(?:class|struct) (?:%s) (\w+)' % keyword, scontent):
                dirmap += '    { "%s", "%s/%s"},\n' % (match, component, sfile)
                classcount += 1
    return dirmap

for component in components:
    keyword = components[component]
    mapdata += exportscan('src/%s' % component, keyword, 'Qt%s' % component.capitalize())

mapdata += '};\n'
mapdata += 'static const int qclass_lib_count = %d;\n\n' % classcount
mapdata += '#endif\n'

with open(mapoutput, 'wb') as f:
    sys.stdout.write('-- Writing: %s\n' % mapoutput)
    if sys.version_info[0] == 3:
        f.write(bytes(mapdata, 'utf-8'))
    else:
        f.write(mapdata)
