#include <emscripten.h>
#include <emscripten/bind.h>

#include <string>
#include <cstdio>

#include "vm.hpp"
using namespace emscripten;

// Wrapper functions

vm::dword_t r_(vm::cpu::CpuState& arr, unsigned n) {
  if (n < vm::cpu::N_GPRS) {
    return arr.r[n];
  }
  return -1;
}

vm::dword_t pc_(vm::cpu::CpuState& arr) {
  return arr.pc;
}


void WriteROM_ (vm::VirtualComputer& arr,long ptr, size_t rom_size) {
  auto p = (vm::byte_t *)ptr;
  arr.WriteROM(p, rom_size);
}


bool AddGKey_(vm::VirtualComputer& arr, unsigned slot, vm::keyboard::GKeyboard& d ) {
  return arr.AddDevice(slot, d);
}

bool AddCDA_(vm::VirtualComputer& arr, unsigned slot, vm::cda::CDA& d ) {
  return arr.AddDevice(slot, d);
}

void WriteTexture_(vm::cda::CDA& arr, long ptr) {
  auto p = (vm::byte_t *)ptr;
  arr.ToRGBATexture((vm::dword_t*)p); // TODO Perhaps we need a special function for this case
}

EMSCRIPTEN_BINDINGS(rc3200_vm) {
    function("LoadROM",     &vm::aux::LoadROM);
    function("Register",    &r_);
    
    class_<vm::cpu::CpuState>("CpuState")
      .constructor<>()
      .function("R",          &r_)
      .function("PC",         &pc_)
      ;

    class_<vm::VirtualComputer>("VirtualComputer")
      .constructor<int>()
      .function("Reset",      &vm::VirtualComputer::Reset)
      .function("WriteROM",   &WriteROM_)
      .function("AddKeyboard",&AddGKey_)
      .function("AddCDA",     &AddCDA_)
      .function("RemoveDevice",  &vm::VirtualComputer::RemoveDevice)
      .function("CPUState",   &vm::VirtualComputer::CPUState)
      .function("Clock",      &vm::VirtualComputer::Clock)
      .function("Step",       &vm::VirtualComputer::Step)
      .function("Tick",       &vm::VirtualComputer::Tick)
      ;

    class_<vm::cda::CDA>("CDA")
      .constructor<int, int>()
      .function("VideoMode",  &vm::cda::CDA::VideoMode)
      .function("isTextMode", &vm::cda::CDA::isTextMode)
      .function("isUserPalette",  &vm::cda::CDA::isUserPalette)
      .function("isUserFont", &vm::cda::CDA::isUserFont)
      .function("VSync",      &vm::cda::CDA::VSync)
      .function("ToRGBATexture",  &WriteTexture_, allow_raw_pointers())
      ;
    
    class_<vm::keyboard::GKeyboard>("GKeyboard")
      .constructor<int, int>()
      .function("PushKeyEvent",  &vm::keyboard::GKeyboard::PushKeyEvent)
      ;

    enum_<vm::keyboard::SCANCODES>("SCANCODES")
      .value("BACKSPACE", vm::keyboard::BACKSPACE)
      .value("RETURN", vm::keyboard::RETURN)
      .value("INSERT", vm::keyboard::INSERT)
      .value("DELETE", vm::keyboard::DELETE)
      .value("ESC", vm::keyboard::ESC)
      .value("ARROW_UP", vm::keyboard::ARROW_UP)
      .value("ARROW_DOWN", vm::keyboard::ARROW_DOWN)
      .value("ARROW_LEFT", vm::keyboard::ARROW_LEFT)
      .value("ARROW_RIGHT", vm::keyboard::ARROW_RIGHT)
      .value("SHIFT", vm::keyboard::SHIFT)
      .value("CONTROL", vm::keyboard::CONTROL)
      ;
      
}


