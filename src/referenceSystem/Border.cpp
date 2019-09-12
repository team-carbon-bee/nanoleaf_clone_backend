#include "Border.h"

namespace referenceSystem {


Border::Border(ConfigurationProvider configuration)
    : _configuration(configuration)
{
    _pixels = malloc(sizeof(Color) * _configuration.parameters().ledPerTriangle * numberOfShape)
}

Border::~Border()
{
}

} //referenceSystem