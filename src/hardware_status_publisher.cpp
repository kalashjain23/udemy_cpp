#include "rclcpp/rclcpp.hpp"
#include "udemy_interfaces/msg/hardware_status.hpp"

class HardwareStatusPublisherNode : public rclcpp::Node
{
public:
    HardwareStatusPublisherNode() : Node("hardware_status_publisher")
    {
        this->publisher_ = this->create_publisher<udemy_interfaces::msg::HardwareStatus>("hardware_status", 10);

        this->timer_ = this->create_wall_timer(
            std::chrono::seconds(1),
            std::bind(&HardwareStatusPublisherNode::publish_hardware_status, this));

        RCLCPP_INFO(this->get_logger(), "Hardware status publisher has been started!");
    }

private:
    void publish_hardware_status()
    {
        auto msg = udemy_interfaces::msg::HardwareStatus();
        msg.temperature = 45;
        msg.are_motors_ready = true;
        msg.debug_message = "Nothing special here as well!";

        publisher_->publish(msg);
    }

    rclcpp::Publisher<udemy_interfaces::msg::HardwareStatus>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto hardware_status_publisher = std::make_shared<HardwareStatusPublisherNode>();
    rclcpp::spin(hardware_status_publisher);
    rclcpp::shutdown();
}