#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <memory>

static const char FILE_SEP = '.';

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
        GCovFile(string file_name) : _handle(new ifstream) {
            _handle->open(file_name.c_str(), ios::in|ios::binary);

            if (_handle->is_open()) {
                cout << "successfully opened " << file_name << endl;
            } else {
                cout << "unable to open " << file_name << endl;
            }

            if (check_file_type()) {
                cout << "file type confirmed" << endl;
            }

            _handle->read((char*)&_magic, sizeof(_magic));
            _handle->read((char*)&_version, sizeof(_version));
            _handle->read((char*)&_stamp, sizeof(_stamp));

            cout << "version major: " << _version.major << endl;
            cout << "minor version: " << _version.minor[0] << _version.minor[1] << endl;

            read_record();
        }

        virtual ~GCovFile() {}

    protected:
        bool check_file_type() { return magic() == _magic; }
        virtual uint32_t magic() = 0;

    private:
        void read_record() {
            cout << "[RECORD] reading a record" << endl;
            RecordHeader record_header;
            _handle->read((char*)&record_header, sizeof(record_header));
            cout << "[RECORD] tag is " << record_header.tag << " len is " << record_header.len * 4 << " bytes" << endl;
            char record_buffer[record_header.len * 4];
            _handle->read(record_buffer, record_header.len * 4);
            _records.push_back(Record(record_header, record_buffer));
        }

        struct Version {
            char status;
            char minor[2];
            char major;
        };

        uint32_t _magic;
        Version _version;
        uint32_t _stamp;
        unique_ptr<ifstream> _handle;
        vector<Record> _records;
};

class GCNOFile : public GCovFile {
    public:
        GCNOFile(string file_name) : GCovFile(file_name) {};
        ~GCNOFile() {};
    private:
        static const unsigned MAGIC = 0x67636E6F; // gcno
        uint32_t magic() { return MAGIC; }
};

class GCDAFile : public GCovFile {
    public:
        GCDAFile(string file_name) : GCovFile(file_name) {};
        ~GCDAFile() {};
    private:
        static const unsigned MAGIC = 0x67636461; // gcda
        uint32_t magic() { return MAGIC; }
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

    GCNOFile gcno_file(gcno_filename);
    GCDAFile gcda_file(gcda_filename);

    return EXIT_SUCCESS;
}
