#include <stdio.h>

#include <pdif_cli/pdif_cli_config.hpp>
#include <pdif/test.hpp>

int main(int argc, char** argv)
{
    printf("pdif_cli version %s.%s.%s\n", PDIF_CLI_VERSION_MAJOR, PDIF_CLI_VERSION_MINOR, PDIF_CLI_VERSION_PATCH);
    printf("%d", test());
    return 0;
}