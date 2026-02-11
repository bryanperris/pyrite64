/**
* @copyright 2025 - Max Bebök
* @license MIT
*/
#include "filePicker.h"

#include <atomic>
#include <mutex>
#include <SDL3/SDL.h>

#include "../context.h"

namespace
{
  constinit bool isPickerOpen{false};
  std::mutex mtxResult{};
  constinit std::atomic_bool hasResult{false};
  constinit std::string result{};

  std::function<void(const std::string&path)> resultUserCb;

  void cbResult(void *userdata, const char * const *filelist, int filter) {
    std::lock_guard lock{mtxResult};
    result = (filelist && filelist[0]) ? filelist[0] : "";
    hasResult = true;
  }
}

bool Utils::FilePicker::open(std::function<void(const std::string&path)> cb, bool isDirectory, const std::string &title) {
  if (isPickerOpen) return false;

  resultUserCb = cb;
  SDL_PropertiesID props = SDL_CreateProperties();
  SDL_SetPointerProperty(props, SDL_PROP_FILE_DIALOG_WINDOW_POINTER, ctx.window);
  //SDL_SetStringProperty(props, SDL_PROP_FILE_DIALOG_LOCATION_STRING, default_location);
  SDL_SetBooleanProperty(props, SDL_PROP_FILE_DIALOG_MANY_BOOLEAN, false);
  if(!title.empty()) {
    SDL_SetStringProperty(props, SDL_PROP_FILE_DIALOG_TITLE_STRING, title.c_str());
  }
  SDL_ShowFileDialogWithProperties(
    isDirectory ? SDL_FILEDIALOG_OPENFOLDER : SDL_FILEDIALOG_OPENFILE,
    cbResult, nullptr, props
  );
  SDL_DestroyProperties(props);

  isPickerOpen = true;
  return true;
}

void Utils::FilePicker::poll()
{
  if (hasResult) {
    mtxResult.lock();
    auto resLocal = result;
    mtxResult.unlock();

    if (!resLocal.empty()) {
      resultUserCb(resLocal);
    }

    isPickerOpen = false;
    hasResult = false;
  }
}
