namespace mahi {
namespace util {

template <typename Arg, typename... Args>
void Csv::write_row(Arg&& arg, Args&&... args) {
    std::ostringstream ss;
    ss << std::setprecision(precision_);
    ss << std::forward<Arg>(arg);
    using expander = int[];
    (void)expander{0, (void(ss << ',' << std::forward<Args>(args)), 0)...};
    ss << "\n";
    write(ss.str());
}

template <typename Container1D>
bool csv_read_row(const std::string& filepath, Container1D& data_out, std::size_t row_offset, std::size_t col_offset) {
    std::string directory, filename, ext, full;
    if (!parse_filepath(filepath, directory, filename, ext, full))
        return false;
    std::ifstream file(full);
    file.precision(6);
    if (!file.is_open())
        return false;
    std::string line_string;
    std::size_t row_r_idx = 0;
    while (std::getline(file, line_string)) {
        if (row_r_idx++ == row_offset) {
            std::istringstream line_stream(line_string);
            std::string value_string;
            std::size_t col_r_idx = 0;
            std::size_t col_w_idx = 0;
            while (std::getline(line_stream, value_string, ',')) {
                if (col_r_idx++ < col_offset)
                    continue;
                if (col_w_idx == data_out.size())
                    break;
                std::istringstream value_stream(value_string);
                value_stream >> data_out[col_w_idx];
                col_w_idx++;
            }
            break;
        }
    }
    return true;
}

template <typename Container2D>
bool csv_read_rows(const std::string& filepath, Container2D& data_out, std::size_t row_offset, std::size_t col_offset) {
    std::string directory, filename, ext, full;
    if (!parse_filepath(filepath, directory, filename, ext, full))
        return false;
    std::ifstream file(full);
    file.precision(6);
    if (!file.is_open())
        return false;
    std::string line_string;
    std::size_t row_r_idx = 0;
    std::size_t row_w_idx = 0;
    while (std::getline(file, line_string))
    {
        if (row_r_idx++ < row_offset)
            continue;
        if (row_w_idx == data_out.size())
            break;
        else {
            std::istringstream line_stream(line_string);
            std::string value_string;
            std::size_t col_r_idx = 0;
            std::size_t col_w_idx = 0;
            while (std::getline(line_stream, value_string, ','))
            {
                if (col_r_idx++ < col_offset)
                    continue;
                if (col_w_idx == data_out[row_w_idx].size())
                    break;
                std::istringstream value_stream(value_string);
                value_stream >> data_out[row_w_idx][col_w_idx];
                col_w_idx++;
            }
            row_w_idx++;
        }
    }
    return true;
}

template <typename Container1D>
bool csv_write_row(const std::string &filepath, const Container1D &data)
{
    File file(filepath, WriteMode::Truncate);
    std::ostringstream ss;
    ss << std::setprecision(6);
    for (size_t j = 0; j < data.size() - 1; ++j)
        ss << data[j] << ",";
    ss << data[data.size() - 1] << "\r\n";
    file.write(ss.str());
    file.close();
    return true;
}

template <typename Container2D>
bool csv_write_rows(const std::string &filepath, const Container2D &data)
{
    File file(filepath, WriteMode::Truncate);
    for (std::size_t i = 0; i < data.size(); i++)
    {
        std::ostringstream ss;
        ss << std::setprecision(6);
        for (size_t j = 0; j < data[i].size() - 1; ++j)
            ss << data[i][j] << ",";
        ss << data[i][data[i].size() - 1] << "\r\n";
        file.write(ss.str());
    }
    file.close();
    return true;
}

template <typename Container1D>
bool csv_append_row(const std::string &filepath, const Container1D &data)
{
    File file(filepath, WriteMode::Append);
    std::ostringstream ss;
    ss << std::setprecision(6);
    for (size_t j = 0; j < data.size() - 1; ++j)
        ss << data[j] << ",";
    ss << data[data.size() - 1] << "\r\n";
    file.write(ss.str());
    file.close();
    return true;
}

template <typename Container2D>
bool csv_append_rows(const std::string &filepath, const Container2D &data)
{
    File file(filepath, WriteMode::Append);
    for (std::size_t i = 0; i < data.size(); i++)
    {
        std::ostringstream ss;
        ss << std::setprecision(6);
        for (size_t j = 0; j < data[i].size() - 1; ++j)
        {
            ss << data[i][j] << ",";
        }
        ss << data[i][data[i].size() - 1] << "\r\n";
        file.write(ss.str());
    }
    file.close();
    return true;
}

} // namespace util
} // namespace mahi