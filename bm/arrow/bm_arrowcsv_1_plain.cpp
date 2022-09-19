#include <arrow/csv/api.h>
#include <arrow/io/api.h>
#include <arrow/ipc/api.h>
#include <arrow/pretty_print.h>
#include <arrow/result.h>
#include <arrow/status.h>
#include <arrow/table.h>

#include "bm_c4corecsv_common.hpp"

#include <iostream>

using arrow::Status;

namespace {

Status RunMain(const char *csv_filename)
{
    std::shared_ptr<arrow::io::ReadableFile> input_file;
    std::shared_ptr<arrow::csv::TableReader> csv_reader;
    std::shared_ptr<arrow::Table> table;
    TIMEDSECTION(runmain);
    {
        TIMEDSECTION(openfile);
        ARROW_ASSIGN_OR_RAISE(input_file, arrow::io::ReadableFile::Open(csv_filename));
    }
    {
        TIMEDSECTION(makereader);
        ARROW_ASSIGN_OR_RAISE(csv_reader, arrow::csv::TableReader::Make(
                                  arrow::io::default_io_context(), input_file,
                                  arrow::csv::ReadOptions::Defaults(),
                                  arrow::csv::ParseOptions::Defaults(),
                                  arrow::csv::ConvertOptions::Defaults()));
    }
    {
        TIMEDSECTION(read);
        ARROW_ASSIGN_OR_RAISE(table, csv_reader->Read());
    }
    return Status::OK();
}
}  // namesparrow::csve


int main(int argc, char** argv)
{
    C4_CHECK(argc > 1);
    const char* csv_filename = argv[1];
    Status st = RunMain(csv_filename);
    if (!st.ok()) {
        std::cerr << st << std::endl;
        return 1;
    }
    return 0;
}
