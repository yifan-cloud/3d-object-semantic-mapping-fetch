#include <ros/ros.h>
#include "Projector.h"

using std::string;

int main (int argc, char** argv)
{
    // NODE 
    string node_name="object_projector";

    // Initialize ROS
    ros::init (argc, argv, node_name);
    ros::NodeHandle * nh = new ros::NodeHandle("~");

    // Parameter variables

    // TOPICS IN
    string pointcloud_topic, boxes_topic, detection_flag_topic, odom_topic;

    // TOPIC OUT 
    string out_topic; 

    // FRAMES
    string camera_frame, robot_frame, global_frame; 

    // CLASSES
    vector<string> class_names;

    // Projection method
    // 0: Naive - median of middle points
    // 1: Preferred - Simple clustering (OR plane segmentation for doors)
    // 2: Advanced - Remove planes and then apply clustering
    // others: defaults to Preferred
    int projection_method = 1;

    bool quiet_mode = false;

    // OTHER 
    // Fetch camera
    double camera_fx = 554.254691191187f;
    double camera_fy = 554.254691191187f;
    double camera_cx = 320.5f;
    double camera_cy = 240.5f;

    // Kinect V2
    //double camera_fx = 1074.01f/2.0f;
    //double camera_fy = 1073.9f/2.0f;
    //double camera_cx = 945.3f/2.0f;
    //double camera_cy = 537.4f/2.0f;

    bool use_mean = false, rotation_optmization = true; 
    double max_proj_dist;

    // Get params

    nh->param("pointcloud_topic", pointcloud_topic, string("/head_camera/depth_registered/points"));    
    nh->param("boxes_topic", boxes_topic, string("/darknet_ros/bounding_boxes"));  
    nh->param("detection_flag_topic", detection_flag_topic, string("/darknet_ros/flag_detection"));
    nh->param("odom_topic", odom_topic, string("/odom"));
    nh->param("out_topic", out_topic, string("/objects_raw"));
    nh->param("camera_frame", camera_frame, string("head_camera_rgb_optical_frame"));
    nh->param("robot_frame", robot_frame, string("/base_link"));
    nh->param("global_frame", global_frame, string("/map"));

    nh->param("classes", class_names, vector<string>());
    nh->param("projection_method", projection_method, 1);
    nh->param("quiet_mode", quiet_mode, false);
    
    nh->param("camera_cx", camera_cx, 320.5);
    nh->param("camera_cy", camera_cy, 240.5);
    nh->param("camera_fx", camera_fx, 554.254691191187);
    nh->param("camera_fy", camera_fy, 554.254691191187);

    nh->param("rotation_optmization", rotation_optmization, true);
    nh->param("use_mean", use_mean, false);
    nh->param("max_proj_distance", max_proj_dist, 7.0);

    // Initialize and set params
    Projector projector(nh, class_names, pointcloud_topic, boxes_topic, odom_topic, detection_flag_topic,out_topic, quiet_mode);
    projector.projection_method = projection_method;
    projector.camera_frame = camera_frame;
    projector.robot_frame = robot_frame;
    projector.global_frame = global_frame;
    projector.camera_cx = camera_cx; 
    projector.camera_cy = camera_cy;
    projector.camera_fx = camera_fx;
    projector.camera_fy = camera_fy;
    projector.rotation_optmization = rotation_optmization; 
    projector.max_proj_dist = max_proj_dist;

    ROS_INFO_STREAM("\n CAMERA FRAME: "+ camera_frame );
    ROS_INFO_STREAM("\n MAX PROJECTION DISTANCE: "+ std::to_string( max_proj_dist ));

    projector.execute();
}
