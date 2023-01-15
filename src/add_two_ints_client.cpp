#include "rclcpp/rclcpp.hpp"
#include "example_interfaces/srv/add_two_ints.hpp"

class AddTwoIntsClientNode : public rclcpp::Node
{
public:
    AddTwoIntsClientNode() : Node("add_two_ints_client")
    {
        // create a thread pool if you want to send multiple requests (vector of threads)
        thread_ = std::thread(std::bind(&AddTwoIntsClientNode::callAddTwoIntsService, this, 1, 4));
    }

    void callAddTwoIntsService(int a, int b)
    {
        auto client_ = this->create_client<example_interfaces::srv::AddTwoInts>("add_two_ints");
        while(!client_->wait_for_service(std::chrono::seconds(1)))
        {
            RCLCPP_WARN(this->get_logger(), "Waiting for the service to start");
        }

        auto request_ = std::make_shared<example_interfaces::srv::AddTwoInts::Request>();
        request_->a = a;
        request_->b = b;

        auto future_ = client_->async_send_request(request_);

        try
        {
            auto response_ = future_.get();
            RCLCPP_INFO(this->get_logger(), "%d + %d = %ld", a, b, response_->sum);
        }
        catch(std::exception &e)
        {
            RCLCPP_ERROR(this->get_logger(), "Service call failed!");
        }
    }

private:
    std::thread thread_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<AddTwoIntsClientNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
}
