#pragma once

#define SAFE_FREE(x) do { if (x) { delete x; x = nullptr; } } while (0)
#define SAFE_FREE_ARRAY(x) do { if (x) { delete [] x; x = nullptr; } } while (0)
