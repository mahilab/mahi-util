#include <Mahi/Util/Device.hpp>
#include <Mahi/Util/Logging/Log.hpp>

namespace mahi {
namespace util {

Device::Device() :
    enabled_(false),
    open_(false),
    name_("UNNAMED_DEVICE")
{ }

Device::Device(const std::string& name) :
    enabled_(false),
    open_(false),
    name_(name)
{ }

Device::~Device() { }

bool Device::enable() {
    if (enabled_) {
        LOG(Warning) << get_name() << " already enabled";
        return true;
    }
    if (on_enable()) {
        LOG(Verbose) << "Enabled " << get_name();
        enabled_ = true;
        return true;
    }
    else {
        LOG(Error) << "Failed to enabled " << get_name();
        enabled_ = false;
        return false;
    }
}

bool Device::disable() {
    if (!enabled_) {
        LOG(Warning) << get_name() << " already disabled";
        return true;
    }
    if (on_disable()) {
        LOG(Verbose) << "Disabled " << get_name();
        enabled_ = false;
        return true;
    }
    else {
        LOG(Error) << "Failed to disable " << get_name();
        enabled_ = true;
        return false;
    }
}

bool Device::is_enabled() const {
    return enabled_;
}

bool Device::open() {
    if (open_) {
        LOG(Warning) << "Device " << get_name() << " already open";
        return true;
    }
    if (on_open()) {
        LOG(Info) << "Opened Device " << get_name();
        open_ = true;
        return true;
    }
    else {
        LOG(Error) << "Failed to open Device " << get_name();
        open_ = false;
        return false;
    }
}

bool Device::close() {
    if (!open_) {
        LOG(Warning) << "Device " << get_name() << " already closed";
        return true;
    }
    if (on_close()) {
        LOG(Info) << "Closed Device " << get_name();
        open_ = false;
        return true;
    }
    else {
        LOG(Error) << "Failed to close Device " << get_name();
        open_ = true;
        return false;
    }
}

bool Device::is_open() const {
    return open_;
}

const std::string& Device::get_name() const {
    return name_;
}

void Device::set_name(const std::string& name) {
    name_ = name;
}

bool Device::on_open() {
    return true;
}

bool Device::on_close() {
    return true;
}

bool Device::on_enable() {
    return true;
}

bool Device::on_disable() {
    return true;
}

} // namespace daq
} // namespace mahi
