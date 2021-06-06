#include <iostream>
#include <Eigen/Core>
#include <Eigen/Dense>

#include <array>
#include <vector>

class LS{
private:
    
};

void generate_pos(std::vector<std::array<double,7>> &container){
    std::array<double,7> test1 = {1,1,0,4,5,6,7};
    std::array<double,7> test2 = {11,12,13,14,15,16,17};
    std::array<double,7> test3 = {21,22,23,24,25,26,27};
    container.push_back(test1);
    container.push_back(test2);
    container.push_back(test3);
}

void transform_pos(std::array<double,7>& before_pos, std::vector<std::array<double,7>>& after,
                   Eigen::Vector3d &pretranslate, Eigen::Matrix3d &rotation_matrix){
    //Eigen::Vector3d before_pretranslate(before_pos[0],before_pos[1],before_pos[2]);
    std::cout<<"Begin to do the transform"<<std::endl;
    Eigen::Quaterniond before_quaterniond(before_pos[6],before_pos[3],before_pos[4],before_pos[5]);

    std::array<double,7> after_pos;
    after_pos[0] = before_pos[0] + pretranslate.x();
    after_pos[1] = before_pos[1] + pretranslate.y();
    after_pos[2] = before_pos[2] + pretranslate.z();

    Eigen::Vector3d before_eulerAngle = before_quaterniond.matrix().eulerAngles(2,1,0); //(Z-Y-X RPY)
    Eigen::Vector3d after_eulerAngle = rotation_matrix * before_eulerAngle;

    Eigen::Quaterniond after_quaterniond;
    after_quaterniond = Eigen::AngleAxisd(before_eulerAngle[0], Eigen::Vector3d::UnitZ()) *
                        Eigen::AngleAxisd(before_eulerAngle[1], Eigen::Vector3d::UnitY()) *
                        Eigen::AngleAxisd(before_eulerAngle[2], Eigen::Vector3d::UnitX());

    after_pos[3] = after_quaterniond.x();
    after_pos[4] = after_quaterniond.y();
    after_pos[5] = after_quaterniond.z();
    after_pos[6] = after_quaterniond.w();
    
    after.push_back(after_pos);
}

int main() {
    std::vector<std::array<double,7>> before_pos;
    std::vector<std::array<double,7>> after_pos;
    generate_pos(before_pos);

    Eigen::Matrix3d rotation_matrix = Eigen::Matrix3d::Identity();

    Eigen::AngleAxisd rotation_vector (M_PI/2,Eigen::Vector3d (0,0,1));

    rotation_matrix = rotation_vector.toRotationMatrix();
    std::cout<<"Roatation Matrix is: "<<std::endl;
    std::cout<<rotation_matrix.matrix()<<std::endl;

    Eigen::Vector3d pretranslate(1,1,1);

    for(int i = 0; i < before_pos.size(); i++){
        transform_pos(before_pos[i],after_pos,pretranslate,rotation_matrix);
    }

    for(int i = 0; i < before_pos.size(); i++){
        std::cout<<"****************************"<<std::endl;
        std::cout<<"The NO."<<i<<" begin pose: "<<std::endl;
        std::cout<<"X is : "<<before_pos[i][0]<<std::endl;
        std::cout<<"Y is : "<<before_pos[i][1]<<std::endl;
        std::cout<<"Z is : "<<before_pos[i][2]<<std::endl;

        std::cout<<"Before orientation is: "<<std::endl;
        std::cout<<"W is : "<<before_pos[i][6]<<std::endl;
        std::cout<<"X is : "<<before_pos[i][3]<<std::endl;
        std::cout<<"Y is : "<<before_pos[i][4]<<std::endl;
        std::cout<<"Z is : "<<before_pos[i][5]<<std::endl;

        std::cout<<"After pos is: "<<std::endl;
        std::cout<<"X is : "<<after_pos[i][0]<<std::endl;
        std::cout<<"Y is : "<<after_pos[i][1]<<std::endl;
        std::cout<<"Z is : "<<after_pos[i][2]<<std::endl;

        std::cout<<"After orientation is: "<<std::endl;
        std::cout<<"W is : "<<after_pos[i][6]<<std::endl;
        std::cout<<"X is : "<<after_pos[i][3]<<std::endl;
        std::cout<<"Y is : "<<after_pos[i][4]<<std::endl;
        std::cout<<"Z is : "<<after_pos[i][5]<<std::endl;


    }

    return 0;
}
