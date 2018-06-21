import platform
import os
import importlib
import sys
platform_imports = {}

class SplitImporter(object):
    """SplitImporter

    Overrides default python importer. Checks if package is split across different directories in path.
    """
    def find_module(self, module_name, package_path=None):
        """find_module

        No doc string, file a bug report.

        Parameters:
                module_name: No docstring, file a bug report.
                package_path: No docstring, file a bug report.
        """
        return self

    def load_module(self, module_path):
        """load_module

        Load module and if it is a package extend module path.

        Parameters:
                module_path: No docstring, file a bug report.
        """
        sys.meta_path.remove(self)
        pck = importlib.import_module(module_path)
        sys.meta_path.append(self)
        full_path = os.path.join(*module_path.split('.'))

        # On package import,
        # check if pakcage is not split in multiple paths.
        try:
            for ppath in sys.path:
                if ppath != pck.__path__[0].rstrip('/'+full_path):
                    additional_path = os.path.join(ppath, full_path)
                    if os.path.exists(additional_path):
                        pck.__path__.append(additional_path)
        except AttributeError:
            # If __path__ is not available
            # then it's not a package.
            pass
        return pck

platform_imports['Android'] = 'Android'
platform_name = os.environ.get('AEX_PYTHON_PLATFORM')
if not platform_name:
    system_id = platform.system()
    if system_id in platform_imports:
        platform_name = platform_imports[system_id]
    else:
        raise RuntimeError("Platform {0} is not supported by this application.".format(system_id))
# Use SplitImporter on Android on default since python code is kept in assets while
# native code is unpacked by Android.
if platform_name == "Android":
    sys.meta_path.append(SplitImporter())
platform_dependant = importlib.import_module("."+platform_name, sys.modules[__name__].__package__)
system = getattr(platform_dependant, 'system')
