#include "Operation.h"

int main(int argc, const char *argv[]) {
	VideoCapture capture(argv[1]);

	if(!capture.isOpened()) {
		writeLogMessage(LOG_LEVEL_INFO, format("%s could not be opened!", argv[1]).c_str());
		return EXIT_FAILURE;
	}

	writeLogMessage(LOG_LEVEL_INFO, format("Capture was initiated in %s", argv[1]).c_str());
	Operation operation(capture);
	return EXIT_SUCCESS;
}
