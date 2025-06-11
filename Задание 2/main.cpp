/*
Задание 2

Дата выполнения: 10 марта 2024
*/

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>

// Структуры
// Структура данных RPC
struct RPC_D {
    double LINE_OFF;
    double SAMP_OFF;
    double LAT_OFF;
    double LONG_OFF;
    double HEIGHT_OFF;
    double LINE_SCALE;
    double SAMP_SCALE;
    double LAT_SCALE;
    double LONG_SCALE;
    double HEIGHT_SCALE;
};

// Структура коэффициентов
struct RPC_C {
    std::vector<double> LINE_NUM_COEFF;
    std::vector<double> LINE_DEN_COEFF;
    std::vector<double> SAMP_NUM_COEFF;
    std::vector<double> SAMP_DEN_COEFF;
};

// Функции
double Solve(const std::vector<double>& LINE, double P, double L, double H) {
    double result;
    result = (
        LINE[0] + LINE[1] * L + LINE[2] * P + LINE[3] * H +
        LINE[4] * L * P + LINE[5] * L * H + LINE[6] * P * H + LINE[7] * L * L +
        LINE[8] * P * P + LINE[9] * H * H + LINE[10] * P * L * H + LINE[11] * L * L * L +
        LINE[12] * L * P * P + LINE[13] * L * H * H + LINE[14] * L * L * P + LINE[15] * P * P * P +
        LINE[16] * P * H * H + LINE[17] * L * L * H + LINE[18] * P * P * H + LINE[19] * H * H * H
        );
    return result;
}

std::vector<std::pair<double, double>> sol_RPC(const std::string& RPC_P, double lat, double lon, double hei, const std::vector<std::pair<RPC_DATA, RPC_COEFF>>& RPC) {
    RPC_D rpc_d = RPC[0].first;
    RPC_COEFF rpc_coeff = RPC[0].second;

    std::vector<std::pair<double, double>> coords(1);
    double P, L, H;
    P = (lat - rpc_d.LAT_OFF) / rpc_d.LAT_SCALE;
    L = (lon - rpc_d.LONG_OFF) / rpc_d.LONG_SCALE;
    H = (hei - rpc_d.HEIGHT_OFF) / rpc_d.HEIGHT_SCALE;

    std::vector<double> LINE_NUM_COEFF = rpc_c.LINE_NUM_COEFF;
    std::vector<double> LINE_DEN_COEFF = rpc_c.LINE_DEN_COEFF;
    std::vector<double> SAMP_NUM_COEFF = rpc_c.SAMP_NUM_COEFF;
    std::vector<double> SAMP_DEN_COEFF = rpc_c.SAMP_DEN_COEFF;

    double s1, s2, s3, s4;
    s1 = Solve(LINE_NUM_COEFF, P, L, H);
    s2 = Solve(LINE_DEN_COEFF, P, L, H);
    s3 = Solve(SAMP_NUM_COEFF, P, L, H);
    s4 = Solve(SAMP_DEN_COEFF, P, L, H);

    double rn, cn;
    rn = s1 / s2;
    cn = s3 / s4;

    double row, column;
    row = rpc_d.LINE_OFF + rn * rpc_d.LINE_SCALE;
    column = cn * rpc_d.SAMP_SCALE + rpc_d.SAMP_OFF;

    coords[0].first = row;
    coords[0].second = column;

    return coords;
}

std::vector<std::pair<RPC_D, RPC_C>> inic_RPC(const std::string& RPC_P) {
    RPC_D rpc_d;
    RPC_C rpc_c;

    std::ifstream file(RPC_P);
    if (!file) {
        std::cerr << "Failed to open RPC file: " << std::endl;
        file.close();
        exit(1);
    }

    std::vector<double> LINE_NUM_COEFF(20);
    std::vector<double> LINE_DEN_COEFF(20);
    std::vector<double> SAMP_NUM_COEFF(20);
    std::vector<double> SAMP_DEN_COEFF(20);

    rpc_c.LINE_NUM_COEFF = LINE_NUM_COEFF;
    rpc_c.LINE_DEN_COEFF = LINE_DEN_COEFF;
    rpc_c.SAMP_NUM_COEFF = SAMP_NUM_COEFF;
    rpc_c.SAMP_DEN_COEFF = SAMP_DEN_COEFF;

    std::string line;
    while (getline(file, line)) {
        std::istringstream iss(line);
        std::string NAME_C;
        iss >> NAME_C;

        char sign;
        iss >> sign;

        std::string temp_value;
        iss >> temp_value;
        double value;
        value = std::stod(temp_value);

        if (sign == '-') {
            value = -value;
        }

        char last_elem = line.back();
        if (std::isdigit(last_elem)) {
            int t1 = NAME_C.rfind("_"), t2 = NAME_C.rfind(":");
            std::string get_index = NAME_C.substr(t1 + 1, t2 - t1 - 1);
            int index;
            try {
                index = std::stoi(get_index);
            }
            catch (...) {
                std::cout << "ERROR: Some data in the file is entered incorrectly!";
                exit(1);
            }

            std::string name = NAME_C.substr(0, t1);
            if (name == "LINE_NUM_COEFF") {
                rpc_c.LINE_NUM_COEFF[index - 1] = value;
            }
            else if (name == "LINE_DEN_COEFF") {
                rpc_c.LINE_DEN_COEFF[index - 1] = value;
            }
            else if (name == "SAMP_NUM_COEFF") {
                rpc_c.SAMP_NUM_COEFF[index - 1] = value;
            }
            else if (name == "SAMP_DEN_COEFF") {
                rpc_c.SAMP_DEN_COEFF[index - 1] = value;
            }
        }
        else {
            if (NAME_C == "LINE_OFF:") {
                rpc_d.LINE_OFF = value;
            }
            else if (NAME_C == "SAMP_OFF:") {
                rpc_d.SAMP_OFF = value;
            }
            else if (NAME_C == "LAT_OFF:") {
                rpc_d.LAT_OFF = value;
            }
            else if (NAME_C == "LONG_OFF:") {
                rpc_d.LONG_OFF = value;
            }
            else if (NAME_C == "HEIGHT_OFF:") {
                rpc_data.HEIGHT_OFF = value;
            }
            else if (NAME_C == "LINE_SCALE:") {
                rpc_d.LINE_SCALE = value;
            }
            else if (NAME_C == "SAMP_SCALE:") {
                rpc_d.SAMP_SCALE = value;
            }
            else if (NAME_C == "LAT_SCALE:") {
                rpc_d.LAT_SCALE = value;
            }
            else if (NAME_C == "LONG_SCALE:") {
                rpc_d.LONG_SCALE = value;
            }
            else if (NAME_C == "HEIGHT_SCALE:") {
                rpc_d.HEIGHT_SCALE = value;
            }
        }
    }

    std::vector<std::pair<RPC_D, RPC_C>> result;
    result.push_back(std::make_pair(rpc_d, rpc_c));

    file.close();
    return result;
}

// Главная часть программы
int main(int argc, char* argv[]) {
    std::cout << std::fixed << std::setprecision(11);

    if (argc < 5) {
        std::cerr << "ERROR: The number of input values is not equal to 5" << std::endl;
        return 1;
    }

    std::string RPC_P = argv[4];
    std::vector<std::pair<RPC_D, RPC_C>> RPC = inic_RPC(RPC_P);
    RPC_DATA rpc_data = RPC[0].first;
    std::cout << "RPC" << std::endl;
    std::cout << "LINE_OFF: " << rpc_d.LINE_OFF << std::endl;
    std::cout << "SAMP_OFF: " << rpc_d.SAMP_OFF << std::endl;
    std::cout << "LAT_OFF: " << rpc_d.LAT_OFF << std::endl;
    std::cout << "LONG_OFF: " << rpc_d.LONG_OFF << std::endl;
    std::cout << "HEIGHT_OFF: " << rpc_d.HEIGHT_OFF << std::endl;
    std::cout << "LINE_SCALE: " << rpc_d.LINE_SCALE << std::endl;
    std::cout << "SAMP_SCALE: " << rpc_d.SAMP_SCALE << std::endl;
    std::cout << "LAT_SCALE: " << rpc_d.LAT_SCALE << std::endl;
    std::cout << "LONG_SCALE: " << rpc_d.LONG_SCALE << std::endl;
    std::cout << "HEIGHT_SCALE: " << rpc_d.HEIGHT_SCALE << std::endl;
    std::cout << std::endl;

    // Проверка введённых данных из файла на исключение
    try {
        double lat = std::stod(argv[1]);
        double lon = std::stod(argv[2]);
        double hei = std::stod(argv[3]);
        std::vector<std::pair<double, double>> res;
        res = sol_RPC(RPC_P, lat, lon, hei, RPC);
        double row = res[0].first;
        double column = res[0].second;
        std::cout << "Pixel Coords" << std::endl;
        std::cout << "Column: " << column << std::endl;
        std::cout << "Row: " << row << std::endl;
    }
    catch (...) {
        std::cout << "ERROR: data in the file is incorrectly!" << std::endl;
        return 1;
    }
    return 0;
}