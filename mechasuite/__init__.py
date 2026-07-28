import importlib.metadata

try:
    __version__ = importlib.metadata.version("mechasuite")
except importlib.metadata.PackageNotFoundError:
    __version__ = "unknown"

