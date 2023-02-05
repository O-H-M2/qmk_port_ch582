import warnings

try:
    from .LowLevel import API
    from .APIError import *
    from .Parameters import *
    from . import JLink
except Exception:
    import LowLevel.API as API
    from APIError import *
    from Parameters import *
    import JLink

warnings.warn("API module will be deprecated in version 11, use LowLevel module instead.", PendingDeprecationWarning)

