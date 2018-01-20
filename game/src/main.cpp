#include "gui/app/app.h"
#include "util/common.h"

int main(int argc, char** argv) {
  UNUSED(argc);
  UNUSED(argv);

  try {
    mengde::gui::app::App* app = new mengde::gui::app::App(1024, 768, 60);
    app->Run();

    delete app;
  } catch (const char* msg) {
    LOG_FATAL("Caught Exception : %s", msg);
    return 1;
  }

	return 0;
}
