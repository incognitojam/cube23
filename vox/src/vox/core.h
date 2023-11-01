#pragma once

#define BIT(X) (1 << X)

#define VX_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)
