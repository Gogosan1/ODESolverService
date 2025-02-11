// #include "Solution.hpp"

// void Solution::add_result_at_point(double point, std::vector<double> results)
// {
//     data += std::to_string(point);
//     for (int i = 0; i < dimension; i++)
//     {
//         data += " " + std::to_string(results[i]);
//     }
//     data += "\n";
// }

// Solution::Solution(int dimension)
// {
//     this->dimension = dimension;
//     data += "t";
//     for (int i = 0; i < dimension; i++)
//     {
//         data += " y" + std::to_string(i);
//     }
//     data += "\n";
// }

// std::string Solution::get_data()
// {
//     return data;
// }

// // // Определяем как сериализовать объект Solution в JSON
// // void to_json(nlohmann::json& j, const Solution& sol) {
// //     j = nlohmann::json{{"data", sol.data}, {"dimension", sol.dimension}};
// // }

// // // Определяем как десериализовать объект Solution из JSON
// // void from_json(const nlohmann::json& j, Solution& sol) {
// //     j.at("data").get_to(sol.data);
// //     j.at("dimension").get_to(sol.dimension);
// //}