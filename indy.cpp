#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <memory>

const unsigned LE_GCNO_MAGIC = 0x67636E6F; // gcno
const unsigned LE_GCDA_MAGIC = 0x67636461; // gcda
const char FILE_SEP = '.';

using namespace std;

class Unit {
};

class Note {
    public:

    private:
};

struct RecordHeader {
    uint32_t tag;
    uint32_t len;
};

class Record {
    public:
        Record(RecordHeader& header, char* buffer) {
            cout << "[RECORD] creating a record" << endl;
            tag = header.tag;
            memcpy(&first, &buffer[0], 4);
            memcpy(&second, &buffer[4], 8);
            str = string(&buffer[12]);
            cout << "[RECORD] str in record: " << str << endl;
        }
    private:
        uint32_t tag;
        uint32_t first;
        uint64_t second;
        string str;
};

class GCovFile {
    public:
        GCovFile(string file_name) : handle(new ifstream) {
            handle->open(file_name.c_str(), ios::in|ios::binary);

            if (handle->is_open()) {
                cout << "successfully opened " << file_name << endl;
            } else {
                cout << "unable to open " << file_name << endl;
            }

            handle->read((char*)&magic, sizeof(magic));
            handle->read((char*)&version, sizeof(version));
            handle->read((char*)&stamp, sizeof(stamp));

            if (magic == LE_GCNO_MAGIC) {
                cout << "this is a GCNO file" << endl;
            } else if (magic == LE_GCDA_MAGIC) {
                cout << "this is a GCDA file" << endl;
            } else {
                cout << "i don't know what this is" << endl;
            }

            cout << "version major: " << version.major << endl;
            cout << "minor version: " << version.minor[0] << version.minor[1] << endl;

            read_record();
        }

    private:
        void read_record() {
            cout << "[RECORD] reading a record" << endl;
            RecordHeader record_header;
            handle->read((char*)&record_header, sizeof(record_header));
            cout << "[RECORD] tag is " << record_header.tag << " len is " << record_header.len * 4 << " bytes" << endl;
            char record_buffer[record_header.len * 4];
            handle->read(record_buffer, record_header.len * 4);
            records.push_back(Record(record_header, record_buffer));
        }

        struct Version {
            char status;
            char minor[2];
            char major;
        };

        uint32_t magic;
        Version version;
        uint32_t stamp;
        unique_ptr<ifstream> handle;
        vector<Record> records;
};

class GCNOFile {
};

class GCDAFile {
};

int main(int argc, char** argv) {
    if (argc != 2) {
        cout << "usage: " << argv[0] << " program-name" << endl;
        return EXIT_FAILURE;
    }

    string program_name(argv[1]);
    cout << "the program name is " << program_name << endl;

    string gcno_filename = program_name + FILE_SEP + "gcno";
    string gcda_filename = program_name + FILE_SEP + "gcda";

    GCovFile gcno_file(gcno_filename);
    GCovFile gcda_file(gcda_filename);

    return EXIT_SUCCESS;
}
