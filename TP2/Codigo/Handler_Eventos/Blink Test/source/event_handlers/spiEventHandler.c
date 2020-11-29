#include "header/event_handlers/spiEventHandler.h"
static status_pointer spi_status;

static uint8_t on_init;
void spiEventHandler(package *data)
{
    if (!on_init)
    {
        spi_status = SPI_589_get_buttons_status();
        on_init = 1;
    }

    //Capture SPI input
    SPI_589_update_buttons();

    //Updata data
    data->action.down = spi_status->down;
    data->action.left = spi_status->left;
    data->action.right = spi_status->right;
    data->action.rotate = spi_status->rotate;
    data->reset = spi_status->reset;
}
