#include "operation.h"

int main(int argc, const char **argv) {
    VideoCapture capture(argv[1]);

    if (!capture.isOpened()) {
        cout << format("\n%s could not be opened!\n", argv[1]);
        return EXIT_FAILURE;
    }

    Operation *operation = new Operation(capture);
    delete operation;
	
    return EXIT_SUCCESS;
}
