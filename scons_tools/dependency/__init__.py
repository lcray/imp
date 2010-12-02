import scons_tools.utility
import scons_tools.data
import SCons
from SCons.Script import Glob, Dir, File, Builder, Action, Exit, Scanner

def _search_for_deps(context, libname, extra_libs, headers, body, possible_deps):
    for i in range(0,len(possible_deps)+1):
        lc= extra_libs+possible_deps[0:i]
        #print "Trying "+ str(i) +" with " +str(lc)
        olibs= context.env.get('LIBS', [])
        context.env.Append(LIBS=lc)
        ret=context.sconf.CheckLibWithHeader(libname, header=headers, call=body, language='CXX',
                                             autoadd=False)
        context.env.Replace(LIBS=olibs)
        if ret:
            return (True, [libname]+lc)
    return (False, None)

def add_dependency_link_flags(env, dependencies):
    for d in dependencies:
        env.Append(LIBS=scons_tools.data.get(env).dependencies[d].libs)

def check_lib(context, lib, header, body="", extra_libs=[]):
    oldflags= context.env.get('LINKFLAGS')
    context.env.Replace(LINKFLAGS=context.env['IMP_BIN_LINKFLAGS'])
    if type(lib) == list:
        ret=_search_for_deps(context, lib[0], lib[1:], header, body, extra_libs)
    else:
        ret=_search_for_deps(context, lib, [], header, body, extra_libs)
    if not ret[0]:
        context.env.Replace(LINKFLAGS=oldflags)
        return ret
    if context.env['IMP_BUILD_STATIC']:
        scons_tools.utility.make_static_build(context.env)
        if type(lib) == list:
            bret=_search_for_deps(context, lib[0], lib[1:], header, body, extra_libs)
        else:
            bret=_search_for_deps(context, lib, [], header, body, extra_libs)
        scons_tools.utility.unmake_static_build(context.env)
        # should be the sum of the two
        if bret[0]:
            context.env.Replace(LINKFLAGS=oldflags)
            return (bret[0], ret[1]+bret[1])
        else:
            context.env.Replace(LINKFLAGS=oldflags)
            return (False, None)
    context.env.Replace(LINKFLAGS=oldflags)
    return  (True, ret[1])

def get_dependency_string(name):
    lname= name.lower()
    nname=lname.replace(".", "_")
    return nname


def add_external_library(env, name, lib, header, body="", extra_libs=[]):
    lcname= get_dependency_string(name)
    ucname= lcname.upper()
    variables=[lcname, lcname+"libs"]
    def _check(context):
        if context.env[lcname] == "no":
            context.Message('Checking for '+name+' ...')
            context.Result("disabled")
            scons_tools.data.get(context.env).add_dependency(name, variables=variables,
                                                             ok=False)
            return False
        elif context.env[lcname] == "yes":
            context.Message('Checking for '+name+' ...')
            if context.env.get(lcname+"libs", None) is None:
                context.Result("disabled, libs not specified")
                scons_tools.data.get(context.env).add_dependency(name, variables=variables,
                                                                 ok=False)
                return False
            else:
                val=context.env[lcname+'libs'].split(":")
                #print val
                context.Result(" ".join(val))
                scons_tools.data.get(context.env).add_dependency(name, variables=variables,
                                                                 libs=val)
                return True
        else:
            ret= check_lib(context, lib=lib, header=header,
                           body=body,
                           extra_libs=extra_libs)
            context.Message('Checking for '+name+' ...')
            if ret[0]:
                scons_tools.data.get(context.env).add_dependency(name, variables=variables,
                                                                 libs=ret[1])
                context.Result(" ".join(ret[1]))
            else:
                context.Result(False)
                scons_tools.data.get(context.env).add_dependency(name,
                                                                 variables=variables,
                                                                 ok=False)
            return ret[0]
    vars = env['IMP_VARIABLES']
    vars.Add(SCons.Variables.EnumVariable(lcname, 'Whether to use the '+name+' package', "auto", ["yes", "no", "auto"]))
    vars.Add(lcname+'libs', 'Libs to link against when using '+name+'. Needed which "'+lcname+'" is "yes".', None)
    vars.Update(env)
    if not env.GetOption('help'):
        custom_tests = {'CheckThisLib':_check}
        conf = env.Configure(custom_tests=custom_tests)
    #if not env.GetOption('clean') and not env.GetOption('help'):
        if conf.CheckThisLib():
            env.Append(IMP_ENABLED=[name])
            env.Append(IMP_CONFIGURATION=[lcname+"='yes'"])
            env.Append(IMP_CONFIGURATION=[lcname+"libs='"+":".join(scons_tools.data.get(env).dependencies[name].libs)+"'"])
        else:
            env.Append(IMP_DISABLED=[name])
        conf.Finish()
