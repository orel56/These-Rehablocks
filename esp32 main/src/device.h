

class Device {
    Device::Device();
};

class DeviceList{
    public:
        DeviceList();

        uint8_t get_free_addr();

        bool create_new_device();
};