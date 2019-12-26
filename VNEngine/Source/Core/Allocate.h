#ifndef ALLOCATE_H
#define ALLOCATE_H

#include <Core/DataTypes.h>

namespace vne
{

// ============================================================================

/* Aligned memory allocation */
void* aligned_alloc(Uint32 size, Uint32 align = 4);
/* Free allocated memory */
void aligned_free(void* ptr);

// ============================================================================

}

#endif