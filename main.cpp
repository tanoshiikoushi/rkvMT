#include <cstdio>
#include <experimental/filesystem>
#include <string>
#include <cstdlib>
#include <fstream>
#include <ctime>
#include <chrono>
#include <cstring>
#include <algorithm>
#include <unordered_map>

namespace fs = std::experimental::filesystem;

//crc32 code
/*-
 *  COPYRIGHT (C) 1986 Gary S. Brown.  You may use this program, or
 *  code or tables extracted from it, as desired without restriction.
 */
 const unsigned long crc32_tab[] = {
	0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f,
	0xe963a535, 0x9e6495a3,	0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
	0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2,
	0xf3b97148, 0x84be41de,	0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
	0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec,	0x14015c4f, 0x63066cd9,
	0xfa0f3d63, 0x8d080df5,	0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
	0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b,	0x35b5a8fa, 0x42b2986c,
	0xdbbbc9d6, 0xacbcf940,	0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
	0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423,
	0xcfba9599, 0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
	0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d,	0x76dc4190, 0x01db7106,
	0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
	0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d,
	0x91646c97, 0xe6635c01, 0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
	0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950,
	0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
	0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7,
	0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
	0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa,
	0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
	0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81,
	0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
	0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683, 0xe3630b12, 0x94643b84,
	0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
	0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb,
	0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
	0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 0xd6d6a3e8, 0xa1d1937e,
	0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
	0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55,
	0x316e8eef, 0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
	0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28,
	0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
	0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f,
	0x72076785, 0x05005713, 0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
	0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 0x86d3d2d4, 0xf1d4e242,
	0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
	0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69,
	0x616bffd3, 0x166ccf45, 0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
	0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc,
	0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
	0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693,
	0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
	0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};

unsigned long crc32(char* buf, size_t size)
{
    char* p = buf;
    unsigned long crc;

    crc = ~0U;
    while (size--)
        crc = crc32_tab[(crc ^ *p++) & 0xFF] ^ (crc >> 8);
    return crc ^ ~0U;
}

/*void testCRC(char* data, size_t size)
{
    printf("CRC32: %.4lX\n", crc32(data, size));
}*/

//my code starts here :D
const unsigned char ARG_COUNT_MAX = 5;
const unsigned char INPUT_CHARACTER_MAX = 0xff;

const unsigned int FILE_NAME_LENGTH = 0x20;
const unsigned int FILE_ENTRY_PHYSICAL_SIZE = 0x40;

const std::string pp_dir = "_PP_Files\\";

struct UngeneratedFileEntry
{
    std::string file_path;
    std::string file_name;
    std::string relative_dir;
    bool symlink = false;
    bool static_file = false;
    bool processed = false;
};

struct UngeneratedDirEntry
{
    std::string relative_dir;
};

struct FileEntry
{
    char* name = new char[FILE_NAME_LENGTH];
    unsigned long directory_index;
    unsigned long long file_size;
    unsigned long physical_data_pointer;
    unsigned long crc32;
    time_t timestamp;
    unsigned long unique_id;
    bool static_file = false;

    //generated only
    char* file_physical_data;
    unsigned long long physical_file_size;
};

const unsigned int DIRECTORY_NAME_LENGTH = 0x100;
const unsigned int DIRECTORY_ENTRY_PHYSICAL_SIZE = 0x100;

struct DirectoryEntry
{
    //logical
    unsigned long dir_index;
    //physical
    char* name = new char[DIRECTORY_NAME_LENGTH];
};

struct RKVFile
{
    unsigned long file_count;
    FileEntry* files;
    unsigned long directory_count;
    DirectoryEntry* directories;
    unsigned long long data_size;
    char* physical_data;
    bool loaded = false;

    //only for generating rkvs
    bool generated = false;
    bool firstLoad = true;
    std::vector<UngeneratedFileEntry> files_to_gen;
    std::vector<UngeneratedDirEntry> dirs_to_gen;
    std::string in_base;

    //only when extracting
    std::vector<FileEntry> pp_files;
    std::vector<std::string> pp_file_names;
    unsigned long pp_dir_ind = 0;
    bool contains_pp = false;
};

bool unloadRKV(RKVFile* r)
{
    if (!r->loaded)
    {
        return false;
    }

    if (r->generated)
    {
        delete[] r->files;
        delete[] r->directories;
        r->file_count = 0;
        r->directory_count = 0;
        //HEY YOU NEED TO CHANGE THIS
        free((void*)r->physical_data);
        r->physical_data = nullptr;
        r->data_size = 0;
        r->pp_files.clear();
        r->pp_file_names.clear();
        r->contains_pp = false;
    }

    r->files_to_gen.clear();
    r->dirs_to_gen.clear();
    r->generated = false;
    r->firstLoad = true;
    r->loaded = false;
    return true;
}

bool loadRKV(RKVFile* r, std::string file)
{
    std::fstream in_file;
    in_file.open(file, std::ios::in | std::ios::binary);
    if (in_file.bad() || !in_file.is_open())
    {
        printf("Cannot load file: %s\n", file.c_str());
        return false;
    }
    else
    {
        printf("Loaded!\n");
    }

    in_file.seekg(0, in_file.end);
    in_file.clear();
    r->data_size = in_file.tellg();
    in_file.seekg(0, in_file.beg);
    r->physical_data = (char*)calloc(r->data_size, 1); //allocate data_size bytes
    in_file.read((char*)r->physical_data, r->data_size);
    if (in_file.bad())
    {
        printf("Failed to Read File\n");
        in_file.close();
        unloadRKV(r);
        return false;
    }
    else
    {
        printf("Data Loaded Successfully\n");
        in_file.close();
    }

    //let's get the file and directory counts
    unsigned long long currPtr = r->data_size - 4; //go back 4 bytes
    r->directory_count = (r->physical_data[currPtr] & 0xFF) | ((r->physical_data[currPtr+1] & 0xFF) << 8) |
        ((r->physical_data[currPtr+2] & 0xFF) << 16) | ((r->physical_data[currPtr+3] & 0xFF) << 24);
    if (r->directory_count == 0)
    {
        printf("Malformed File, Resorting to Searching Mode...\n");
        unsigned char b = 0x00;
        while (b == 0x00)
        {
            currPtr--;
            b = r->physical_data[currPtr] & 0xFF;
        }
        //after we find it, let's adjust to the nearest int alignment
        unsigned char shift = (currPtr) % 4;
        currPtr = currPtr - shift;
        r->directory_count = (r->physical_data[currPtr] & 0xFF) | ((r->physical_data[currPtr+1] & 0xFF) << 8) |
            ((r->physical_data[currPtr+2] & 0xFF) << 16) | ((r->physical_data[currPtr+3] & 0xFF) << 24);
    }
    currPtr -= 4;
    r->file_count = (r->physical_data[currPtr] & 0xFF) | ((r->physical_data[currPtr+1] & 0xFF) << 8) |
        ((r->physical_data[currPtr+2] & 0xFF) << 16) | ((r->physical_data[currPtr+3] & 0xFF) << 24);

    printf("Dir Count: %li\nFile Count: %li\n", r->directory_count, r->file_count);

    r->files = new FileEntry[r->file_count];
    r->directories = new DirectoryEntry[r->directory_count];
    r->loaded = true;
    r->generated = true;

    long long dir_index = r->directory_count - 1;
    r->pp_dir_ind = 0;

    for (; dir_index >= 0; dir_index--)
    {
        currPtr -= DIRECTORY_ENTRY_PHYSICAL_SIZE;
        r->directories[dir_index].dir_index = dir_index;
        memcpy(r->directories[dir_index].name, (void*)(r->physical_data + currPtr), DIRECTORY_NAME_LENGTH);

        if (strcmp(r->directories[dir_index].name, pp_dir.c_str()) == 0)
        {
            r->pp_dir_ind = r->directories[dir_index].dir_index;
            r->contains_pp = true;
            printf("pp_dir_ind = %lu\n", r->pp_dir_ind);
        }
    }

    unsigned long long u_count = 0;
    for (long long f = r->file_count - 1; f >= 0; f--)
    {
        currPtr -= FILE_ENTRY_PHYSICAL_SIZE;
        memcpy(r->files[f].name, (void*)(r->physical_data + currPtr), FILE_NAME_LENGTH);
        r->files[f].directory_index = (r->physical_data[currPtr + 0x20] & 0xFF) | ((r->physical_data[currPtr + 0x21] & 0xFF) << 8) |
            ((r->physical_data[currPtr + 0x22] & 0xFF) << 16) | ((r->physical_data[currPtr + 0x23] & 0xFF) << 24);
        r->files[f].file_size = (unsigned long long)((r->physical_data[currPtr + 0x24] & 0xFF) | ((r->physical_data[currPtr + 0x25] & 0xFF) << 8) |
            ((r->physical_data[currPtr + 0x26] & 0xFF) << 16) | ((r->physical_data[currPtr + 0x27] & 0xFF) << 24) |
            ((unsigned long long)(r->physical_data[currPtr + 0x28] & 0xFF) << 32) | ((unsigned long long)(r->physical_data[currPtr + 0x29] & 0xFF) << 40) |
            ((unsigned long long)(r->physical_data[currPtr + 0x2A] & 0xFF) << 48) | ((unsigned long long)(r->physical_data[currPtr + 0x2B] & 0xFF) << 56));
        r->files[f].physical_data_pointer = (r->physical_data[currPtr + 0x2C] & 0xFF) | ((r->physical_data[currPtr + 0x2D] & 0xFF) << 8) |
            ((r->physical_data[currPtr + 0x2E] & 0xFF) << 16) | ((r->physical_data[currPtr + 0x2F] & 0xFF) << 24);
        r->files[f].crc32 = (r->physical_data[currPtr + 0x30] & 0xFF) | ((r->physical_data[currPtr + 0x31] & 0xFF) << 8) |
            ((r->physical_data[currPtr + 0x32] & 0xFF) << 16) | ((r->physical_data[currPtr + 0x33] & 0xFF) << 24);
        r->files[f].timestamp = (time_t)((r->physical_data[currPtr + 0x34] & 0xFF) | ((r->physical_data[currPtr + 0x35] & 0xFF) << 8) |
            ((r->physical_data[currPtr + 0x36] & 0xFF) << 16) | ((r->physical_data[currPtr + 0x37] & 0xFF) << 24));
        if (r->files[f].timestamp < 0)
        {
            printf("Fixed Invalid Timestamp for %s\n", r->files[f].name);
            r->files[f].timestamp = 0;
        }
        r->files[f].unique_id = ((r->physical_data[currPtr + 0x38] & 0xFF) << 24) | ((r->physical_data[currPtr + 0x39] & 0xFF) << 16) |
            ((r->physical_data[currPtr + 0x3A] & 0xFF) << 8) | ((r->physical_data[currPtr + 0x3B] & 0xFF));

        if ((r->files[f].unique_id & 0xFF000000) != 0xFF000000)
        {
            r->files[f].static_file = true;
            u_count++;
        }

        if (r->contains_pp && r->files[f].directory_index == r->pp_dir_ind)
        {
            r->pp_files.push_back(r->files[f]);
        }
    }

    printf("u_count: %.llu\n", u_count);

    return true;
}

bool pokeDir(RKVFile* r, unsigned long index)
{
    if (!r->loaded || !r->generated || (index >= r->directory_count))
    {
        return false;
    }
    printf("Name: %s\nOriginal Index: %li\n", r->directories[index].name, r->directories[index].dir_index);
    return true;
}

bool pokeFile(RKVFile* r, unsigned long index)
{
    if (!r->loaded || !r->generated || (index >= r->file_count))
    {
        return false;
    }
    printf("Name: %s\nDirectory Index: %.2lX\nFile Size: %lli\nData Pointer: %.4lX\nCRC: %.4lX\nTimestamp: %sUnique ID: %.4lX\n",
           r->files[index].name, r->files[index].directory_index, r->files[index].file_size, r->files[index].physical_data_pointer, r->files[index].crc32,
           ctime(&(r->files[index].timestamp)), r->files[index].unique_id);
    return true;
}

struct
{
    bool operator()(FileEntry f1, FileEntry f2) const
    {
        return f1.physical_data_pointer < f2.physical_data_pointer;
    }
} FileEntryPhysicalPointerCompare;

bool extractRKV(RKVFile* r, std::string output_base, long long index, bool preserveCase)
{
    //handle phantom files too! @w@
    //we're gonna need some other structures to handle nicely enumerating all of these

    if (!r->loaded || !r->generated)
    {
        printf("Specified RKV is not loaded\n");
        return false;
    }

    //create base directory
    if (output_base.substr(output_base.length() - 1, 1) != "/" && output_base.substr(output_base.length() - 1, 1) != "\\")
    {
        output_base.push_back('/');
    }

    bool exists = fs::exists(output_base);
    bool is_dir = fs::is_directory(output_base);

    if (!exists)
    {
        fs::create_directories(output_base);
    }
    else if (exists && !is_dir)
    {
        printf("Output location is not a directory\n");
        return false;
    }

    if (index != -1)
    {
        if (r->files[index].physical_data_pointer != 0xFFFFFFFF)
        {
            printf("Specified index is a symlink\n");
            return false;
        }

        //ignore directory here
        std::fstream out_file;
        std::string filename;
        filename = r->files[index].name;

        if (!preserveCase)
        {
            std::transform(filename.begin(), filename.end(), filename.begin(), [](unsigned char c) -> unsigned char { return std::tolower(c); });
            output_base.append(filename);
        }
        else
        {
            output_base.append(filename);
        }

        out_file.open(output_base, std::ios::out | std::ios::binary);
        if (out_file.bad() || !out_file.is_open())
        {
            printf("Cannot open file to write: %s\n", output_base.c_str());
            return false;
        }
        else
        {
            out_file.write(&r->physical_data[r->files[index].physical_data_pointer], r->files[index].file_size);
            out_file.close();

            fs::last_write_time(output_base, std::chrono::system_clock::from_time_t(r->files[index].timestamp));
        }
    }
    else
    {
        std::string ext_dir;
        for (unsigned long j = 0; j < r->directory_count; j++)
        {
            ext_dir.clear();
            ext_dir = output_base + r->directories[j].name;
            exists = fs::exists(ext_dir);
            is_dir = fs::is_directory(ext_dir);

            if (!exists)
            {
                fs::create_directories(ext_dir);
            }
            else if (exists && !is_dir)
            {
                printf("Output location is not a directory\n");
                return false;
            }
        }

        unsigned long* origFiles = new unsigned long[r->file_count];
        unsigned long origCount = 0;
        unsigned long* symLinks = new unsigned long [r->file_count];
        unsigned long symCount = 0;

        std::fstream out_file;
        std::string out_path;
        std::string filename;
        //process regular files
        for (unsigned long long i = 0; i < r->file_count; i++)
        {
            if (r->files[i].physical_data_pointer != 0xFFFFFFFF)
            {
                out_path.clear();
                filename.clear();
                out_path = output_base + r->directories[r->files[i].directory_index].name;

                if (r->files[i].static_file)
                {
                    out_path.append("!");
                }

                filename = r->files[i].name;

                if (!preserveCase)
                {
                    std::transform(filename.begin(), filename.end(), filename.begin(), [](unsigned char c) -> unsigned char { return std::tolower(c); });
                    out_path.append(filename);
                }
                else
                {
                    out_path.append(filename);
                }

                out_file.clear();
                out_file.open(out_path, std::ios::out | std::ios::binary);

                if (out_file.bad() || !out_file.is_open())
                {
                    printf("Cannot open file to write: %s\n", out_path.c_str());

                    delete[] origFiles;
                    delete[] symLinks;
                    return false;
                }
                else
                {
                    out_file.write(&r->physical_data[r->files[i].physical_data_pointer], r->files[i].file_size);
                    out_file.close();

                    fs::last_write_time(out_path, std::chrono::system_clock::from_time_t(r->files[i].timestamp));
                }

                origFiles[origCount] = i;
                origCount++;
            }
            else
            {
                symLinks[symCount] = i;
                symCount++;
            }
        }

        unsigned long desired_time = 0;
        unsigned long search_index = 0;
        bool found = false;
        //process symlinks
        for (unsigned long h = 0; h < symCount; h++)
        {
            desired_time = r->files[symLinks[h]].timestamp;
            found = false;
            for (search_index = 0; search_index < origCount; search_index++)
            {
                if (desired_time == r->files[origFiles[search_index]].timestamp)
                {
                    found = true;
                    break;
                }
            }

            if (!found)
            {
                printf("Symlink '%s' has no linked data\n", r->files[symLinks[h]].name);
                continue;
            }
            else
            {
                //we found a symlink!
                out_path.clear();
                out_path = output_base + r->directories[r->files[symLinks[h]].directory_index].name;
                out_path += "~";
                if (r->files[symLinks[h]].static_file)
                {
                    out_path += "!";
                }

                filename = r->files[symLinks[h]].name;
                if (!preserveCase)
                {
                    std::transform(filename.begin(), filename.end(), filename.begin(), [](unsigned char c) -> unsigned char { return std::tolower(c); });
                    out_path.append(filename);
                }
                else
                {
                    out_path.append(filename);
                }

                out_file.clear();
                out_file.open(out_path, std::ios::out | std::ios::binary);

                if (out_file.bad() || !out_file.is_open())
                {
                    printf("Cannot open file to write: %s\n", out_path.c_str());

                    delete[] origFiles;
                    delete[] symLinks;
                    return false;
                }
                else
                {
                    if (!preserveCase)
                    {
                        char* new_str = new char[FILE_NAME_LENGTH];
                        memcpy(new_str, r->files[origFiles[search_index]].name, FILE_NAME_LENGTH);
                        for (unsigned char l = 0; l < FILE_NAME_LENGTH; l++)
                        {
                            if (new_str[l] == 0x00) { break; }
                            else
                            {
                                new_str[l] = std::tolower(new_str[l]);
                            }
                        }
                        out_file.write(new_str, FILE_NAME_LENGTH);
                    }
                    else
                    {
                        out_file.write(r->files[origFiles[search_index]].name, FILE_NAME_LENGTH);
                    }
                    out_file.close();

                    fs::last_write_time(out_path, std::chrono::system_clock::from_time_t(r->files[symLinks[h]].timestamp));
                }
            }
        }

        delete[] origFiles;
        origCount = 0;
        delete[] symLinks;
        symCount = 0;

        if (r->contains_pp)
        {
            std::sort(r->pp_files.begin(), r->pp_files.end(), FileEntryPhysicalPointerCompare);

            std::fstream order_file;
            order_file.open(output_base + pp_dir + ".order", std::ios::out | std::ios::binary);
            if (order_file.bad() || !order_file.is_open())
            {
                printf("Cannot write file '.order'\n");
                return false;
            }

            for (FileEntry l : r->pp_files)
            {
                order_file.write(l.name, FILE_NAME_LENGTH);
            }

            order_file.close();
        }
    }
    return true;
}

bool loadDirectoryIntoRKV(RKVFile* r, std::string dir_path, std::string spec_path, bool recursive)
{
    if (r->loaded) { return false; }

    //load directory and its files
    fs::path base(dir_path);
    unsigned char base_length = base.string().length();
    std::string temp;

    if (r->firstLoad)
    {
        r->in_base = base.string();
        if (r->in_base.substr(r->in_base.length()-1, 1) != "/" && r->in_base.substr(r->in_base.length()-1, 1) != "\\")
        {
            r->in_base.push_back('\\');
        }

        if (spec_path.substr(0, 2) != "..")
        {
            r->in_base = "";
            printf("Specific Path is not above the data path\n");
            return false;
        }

        if (spec_path.substr(spec_path.length()-1, 1) != "/" && spec_path.substr(spec_path.length()-1, 1) != "\\")
        {
            spec_path.push_back('\\');
        }

        UngeneratedDirEntry spec;
        spec.relative_dir = spec_path;
        r->dirs_to_gen.push_back(spec);

        UngeneratedDirEntry base;
        base.relative_dir = "";
        r->dirs_to_gen.push_back(base);
        r->firstLoad = false;
        r->loaded = true;
    }

    if(recursive)
    {
        fs::path p;
        fs::recursive_directory_iterator spec_i(r->in_base + spec_path);
        for (fs::recursive_directory_iterator curr = begin(spec_i); curr != end(spec_i); curr++)
        {
            p = curr->path();
            p.make_preferred();
            if (fs::is_regular_file(*curr))
            {
                UngeneratedFileEntry f;
                f.file_path = p.string();
                f.file_name = p.filename().string();
                if (p.parent_path().string().length() < base_length)
                {
                    f.relative_dir = spec_path;
                }
                else
                {
                    temp = p.parent_path().string().substr(base_length, p.parent_path().string().length()-base_length);
                    if (!temp.empty())
                    {
                        temp.append("\\");
                        temp = temp.substr(0, temp.length()-1);
                    }
                    f.relative_dir = temp;
                }

                r->files_to_gen.push_back(f);
            }
            else if (fs::is_directory(*curr))
            {
                //create the directory entry
                UngeneratedDirEntry d;
                d.relative_dir = p.string().substr(base_length+1, p.string().length()-base_length).append("\\");

                r->dirs_to_gen.push_back(d);
            }
        }

        fs::recursive_directory_iterator i(r->in_base);
        for (fs::recursive_directory_iterator curr = begin(i); curr != end(i); curr++)
        {
            p = curr->path();
            p.make_preferred();
            if (fs::is_regular_file(*curr))
            {
                UngeneratedFileEntry f;
                f.file_path = p.string();
                f.file_name = p.filename().string();
                if (p.parent_path().string().length() < base_length)
                {
                    f.relative_dir = spec_path;
                }
                else
                {
                    temp = p.parent_path().string().substr(base_length, p.parent_path().string().length()-base_length);
                    if (!temp.empty())
                    {
                        temp.append("\\");
                        temp = temp.substr(0, temp.length()-1);
                    }
                    f.relative_dir = temp;
                }

                r->files_to_gen.push_back(f);
            }
            else if (fs::is_directory(*curr))
            {
                //create the directory entry
                UngeneratedDirEntry d;
                d.relative_dir = p.string().substr(base_length+1, p.string().length()-base_length).append("\\");

                r->dirs_to_gen.push_back(d);
            }
        }
    }
    else
    {
        fs::path p;
        fs::directory_iterator spec_i(r->in_base + spec_path);
        for (fs::directory_iterator curr = begin(spec_i); curr != end(spec_i); curr++)
        {
            p = curr->path();
            p.make_preferred();
            if (fs::is_regular_file(*curr))
            {
                UngeneratedFileEntry f;
                f.file_path = p.string();
                f.file_name = p.filename().string();
                if (p.parent_path().string().length() < base_length)
                {
                    f.relative_dir = spec_path;
                }
                else
                {
                    temp = p.parent_path().string().substr(base_length, p.parent_path().string().length()-base_length);
                    if (!temp.empty())
                    {
                        temp.append("\\");
                        temp = temp.substr(1, temp.length()-1);
                    }
                    f.relative_dir = temp;
                }

                r->files_to_gen.push_back(f);
            }
            else if (fs::is_directory(*curr))
            {
                //create the directory entry
                UngeneratedDirEntry d;
                d.relative_dir = p.string().substr(base_length+1, p.string().length()-base_length).append("\\");

                r->dirs_to_gen.push_back(d);
            }
        }

        fs::directory_iterator i(r->in_base);
        for (fs::directory_iterator curr = begin(i); curr != end(i); curr++)
        {
            p = curr->path();
            p.make_preferred();
            if (fs::is_regular_file(*curr))
            {
                UngeneratedFileEntry f;
                f.file_path = p.string();
                f.file_name = p.filename().string();
                if (p.parent_path().string().length() < base_length)
                {
                    f.relative_dir = spec_path;
                }
                else
                {
                    temp = p.parent_path().string().substr(base_length, p.parent_path().string().length()-base_length);
                    if (!temp.empty())
                    {
                        temp.append("\\");
                        temp = temp.substr(1, temp.length()-1);
                    }
                    f.relative_dir = temp;
                }

                r->files_to_gen.push_back(f);
            }
            else if (fs::is_directory(*curr))
            {
                //create the directory entry
                UngeneratedDirEntry d;
                d.relative_dir = p.string().substr(base_length+1, p.string().length()-base_length).append("\\");

                r->dirs_to_gen.push_back(d);
            }
        }
    }
    return true;
}

struct
{
    bool operator()(UngeneratedFileEntry f1, UngeneratedFileEntry f2) const
    {
        std::string temp1, temp2;
        temp1 = f1.file_name;
        temp2 = f2.file_name;
        std::transform(temp1.begin(), temp1.end(), temp1.begin(), [](unsigned char c) -> unsigned char { return std::tolower(c); });
        std::transform(temp2.begin(), temp2.end(), temp2.begin(), [](unsigned char c) -> unsigned char { return std::tolower(c); });

        return temp1.compare(temp2) < 0 ? true : false;
    }
} UngeneratedFileCompare;

struct
{
    bool operator()(UngeneratedDirEntry d1, UngeneratedDirEntry d2) const
    {
        std::string temp1, temp2;
        temp1 = d1.relative_dir;
        temp2 = d2.relative_dir;
        size_t c1 = std::count(temp1.begin(), temp1.end(), '\\');
        size_t c2 = std::count(temp2.begin(), temp2.end(), '\\');
        bool prior1 = temp1.substr(0, 2) == ".." ? true : false;
        bool prior2 = temp2.substr(0, 2) == ".." ? true : false;
        std::transform(temp1.begin(), temp1.end(), temp1.begin(), [](unsigned char c) -> unsigned char { return std::tolower(c); });
        std::transform(temp2.begin(), temp2.end(), temp2.begin(), [](unsigned char c) -> unsigned char { return std::tolower(c); });

        if ((prior1 | prior2) && (prior1 ^ prior2))
        {
            if (prior1)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        if (c1 != c2)
        {
            return c1 < c2 ? true : false;
        }
        return temp1.compare(temp2) < 0 ? true : false; //temp1 is earlier than temp2
    }
} UngeneratedDirCompare;

//physical file size always increases
unsigned long long getRoundedFileSize_Small(unsigned long long s)
{
    unsigned long long c = s;
    if ((c & 0xFFF) <= 0xA00)
    {
        c = (c & 0xFFFFFFFFFFFFF000) + 0x100; //rounds to nearest 0x100
    }
    else
    {
        c = (c & 0xFFFFFFFFFFFFF000) + 0x1000; //rounds to the nearest 0x1000
    }
    return c;
}

unsigned long long getRoundedFileSize_Large(unsigned long long s)
{
    unsigned long long c = s;
    if ((c & 0xFFF) <= 0x800)
    {
        c = (c & 0xFFFFFFFFFFFFF000) + 0x800; //rounds to nearest 0x100
    }
    else
    {
        c = (c & 0xFFFFFFFFFFFFF000) + 0x1000; //rounds to the nearest 0x1000
    }
    return c;
}

unsigned long long getRoundedPointer(unsigned long long s)
{
    unsigned long long c = s;
    c = (c & 0xFFFFFFFFFFFFF000) + 0x1000; //rounds to the nearest 0x1000
    return c;
}

bool processFileEntry(RKVFile* r, std::unordered_map<std::string, unsigned long>& file_map, std::unordered_map<std::string, unsigned long>& dir_map, UngeneratedFileEntry& f,
                       unsigned long& curr_unique, unsigned long long& curr_data_pointer, unsigned long*& symlink_ids, unsigned long& symlink_count, unsigned long& file_entry)
{
    if (curr_unique > 0x00FFFFFF) { return false; }

    std::fstream file_in;
    if(file_map.contains(f.file_name.substr(0, FILE_NAME_LENGTH-1)))
    {
        if(f.static_file)
        {
            r->files[file_map[f.file_name]].static_file = f.static_file;
        }
        if(f.symlink)
        {
            symlink_ids[symlink_count] = file_entry;
            symlink_count++;
        }
        else
        {
            unsigned long working_id = file_map[f.file_name];
            strncpy(r->files[working_id].name, f.file_name.substr(0, FILE_NAME_LENGTH-1).c_str(), FILE_NAME_LENGTH-1);
            r->files[working_id].name[FILE_NAME_LENGTH-1] = 0x00;
            r->files[working_id].directory_index = dir_map[f.relative_dir];
            if (!r->files[working_id].static_file)
            {
                r->files[working_id].unique_id = curr_unique | 0xFF000000;
            }
            else
            {
                r->files[working_id].unique_id = curr_unique;
            }

            curr_unique++;

            file_in.open(f.file_path, std::ios::in | std::ios::binary);
            if (file_in.bad() || !file_in.is_open())
            {
                printf("Cannot load file: %s\n", f.file_path.c_str());
                unloadRKV(r);
                return false;
            }

            file_in.seekg(0, file_in.end);
            file_in.clear();
            r->files[working_id].file_size = file_in.tellg();
            r->files[working_id].file_physical_data = new char[r->files[working_id].file_size];

            file_in.seekg(0, file_in.beg);
            file_in.read(r->files[working_id].file_physical_data, r->files[working_id].file_size);
            file_in.close();

            r->files[working_id].crc32 = crc32(r->files[working_id].file_physical_data, r->files[working_id].file_size);
            r->files[working_id].timestamp = std::chrono::system_clock::to_time_t(fs::last_write_time(f.file_path));
            //EDIT THE ROUNDING MECHANISM HERE
            r->files[working_id].physical_file_size = getRoundedFileSize_Large(r->files[working_id].file_size);

            r->files[working_id].physical_data_pointer = curr_data_pointer;
            curr_data_pointer += r->files[working_id].physical_file_size;

            r->data_size += r->files[working_id].physical_file_size;

            r->files_to_gen.at(file_entry).processed = true;
        }

        return true;
    }
    printf("File map miss for '%s'\n", f.file_name.c_str());
    return true;
}

bool generateRKV(RKVFile* r)
{
    //sort and then generate the file
    if (!r->loaded || r->generated)
    {
        return false;
    }

    //adjust for symlinks
    for (auto i = r->files_to_gen.begin(); i != r->files_to_gen.end(); i++)
    {
        if(i->file_name.substr(0, 1) == "~")
        {
            i->symlink = true;
            i->file_name = i->file_name.substr(1, FILE_NAME_LENGTH-1);
        }
        else
        {
            i->symlink = false;
        }

        if (i->file_name.substr(0, 1) == "!")
        {
            i->static_file = true;
            i->file_name = i->file_name.substr(1, FILE_NAME_LENGTH-1);
        }
        else
        {
            i->static_file = false;
        }
    }

    std::sort(r->files_to_gen.begin(), r->files_to_gen.end(), UngeneratedFileCompare);
    std::sort(r->dirs_to_gen.begin(), r->dirs_to_gen.end(), UngeneratedDirCompare);

    std::unordered_map<std::string, unsigned long> dir_map;
    dir_map.reserve(r->dirs_to_gen.size());
    r->data_size = 0;
    r->directory_count = 0;
    //hashmap of directories for speed since we're doing 28k+ lookups
    for (UngeneratedDirEntry d : r->dirs_to_gen)
    {
        if (!dir_map.contains(d.relative_dir))
        {
            dir_map[d.relative_dir] = r->directory_count;
            r->directory_count++;
        }
        else
        {
            printf("Duplicate Directory '%s'\n", d.relative_dir.c_str());
        }
    }

    //creates directory entries
    r->directories = new DirectoryEntry[r->directory_count];
    for (auto i = dir_map.begin(); i != dir_map.end(); i++)
    {
        r->directories[i->second].dir_index = i->second;
        strncpy(r->directories[i->second].name, i->first.c_str(), DIRECTORY_NAME_LENGTH-1);
        r->directories[i->second].name[DIRECTORY_NAME_LENGTH-1] = 0x00;
        r->data_size += DIRECTORY_ENTRY_PHYSICAL_SIZE;
    }

    if (dir_map.contains(pp_dir))
    {
        r->pp_dir_ind = dir_map[pp_dir];
        r->contains_pp = true;
    }
    else
    {
        r->contains_pp = false;
    }

    std::unordered_map<std::string, unsigned long> file_map;
    file_map.reserve(r->files_to_gen.size());
    r->file_count = 0;
    //we have a hashmap here just for fast lookups if a file already exists
    for (auto f = r->files_to_gen.begin(); f != r->files_to_gen.end(); f++)
    {
        if (f->file_name == ".order")
        {
            f = r->files_to_gen.erase(f);
            printf(".order filtered!\n");
            f--;
            continue;
        }
        if(!file_map.contains(f->file_name.substr(0, FILE_NAME_LENGTH-1)))
        {
            file_map[f->file_name.substr(0, FILE_NAME_LENGTH-1)] = r->file_count;
            r->file_count++;
            r->data_size += FILE_ENTRY_PHYSICAL_SIZE;
        }
        else
        {
            printf("Duplicate File '%s'\n", f->file_name.substr(0, FILE_NAME_LENGTH-1).c_str());
        }
    }

    unsigned long long curr_data_pointer = 0x00;
    unsigned long curr_unique = 0x00000000;
    //unsigned long working_id = 0;
    std::fstream file_in;
    r->files = new FileEntry[r->file_count];
    unsigned long* symlink_ids = new unsigned long[r->file_count];
    unsigned long symlink_count = 0;

    //PP_DIR MAGIC
    if (r->contains_pp)
    {
        unsigned long pp_count = 0;
        std::fstream order_in;
        order_in.open(r->in_base + pp_dir + ".order", std::ios::in | std::ios::binary);
        if (order_in.bad() || !order_in.is_open())
        {
            printf("Cannot load '.order'!\n%s\n", (r->in_base + pp_dir + ".order").c_str());
            return false;
        }

        order_in.seekg(0, order_in.end);
        pp_count = order_in.tellg() / FILE_NAME_LENGTH;

        if (pp_count > 0)
        {
            order_in.seekg(0, order_in.beg);
            char* data_in = new char[FILE_NAME_LENGTH];
            std::string temp;

            for (unsigned long z = 0; z < pp_count; z++)
            {
                order_in.read(data_in, FILE_NAME_LENGTH);
                temp = data_in;
                r->pp_file_names.push_back(temp);
            }

            order_in.close();

            for (unsigned long p = 0; p < pp_count; p++)
            {
                if (file_map.contains(r->pp_file_names[p]))
                {
                    //unsigned long my_id = file_map[r->pp_file_names[p]];
                    UngeneratedFileEntry my_entry;
                    bool found = false;
                    unsigned long f_id = 0;
                    for (; f_id < r->files_to_gen.size(); f_id++)
                    {
                        if (r->pp_file_names[p].compare(r->files_to_gen.at(f_id).file_name) == 0)
                        {
                            found = true;
                            my_entry = r->files_to_gen.at(f_id);
                            r->files_to_gen.at(f_id).processed = true;
                            break;
                        }
                    }

                    if (found)
                    {
                        bool ret = false;
                        ret = processFileEntry(r, file_map, dir_map, my_entry, curr_unique, curr_data_pointer, symlink_ids, symlink_count, f_id);
                        if (!ret)
                        {
                            printf("Error in file entry processing!\n");
                            return false;
                        }
                    }
                    else
                    {
                        printf("Match not found for pp_file: '%s'\n", r->pp_file_names[p].c_str());
                    }
                }
                else
                {
                    printf("Unknown pp_file: %s\n", r->pp_file_names[p].c_str());
                }
            }
        }
    }

    //REGULAR FILES
    for (unsigned long dir_i = 0; dir_i < r->directory_count; dir_i++)
    {
        if (dir_i == r->pp_dir_ind) { continue; }
        else
        {
            for (unsigned long file_entry = 0; file_entry < r->files_to_gen.size(); file_entry++)
            {
                if (curr_unique > 0x00FFFFFF)
                {
                    printf("Unique IDs Overwhelmed!\n");
                    break;
                }

                UngeneratedFileEntry f = r->files_to_gen.at(file_entry);
                if (f.processed)
                {
                    continue;
                }
                else if (dir_map[f.relative_dir] == dir_i)
                {
                    bool ret = false;
                    ret = processFileEntry(r, file_map, dir_map, f, curr_unique, curr_data_pointer, symlink_ids, symlink_count, file_entry);
                    if (!ret)
                    {
                        printf("Error in file entry processing!\n");
                        return false;
                    }
                }
            }
        }
    }

    //SYMLINK
    unsigned long id_of_link = 0;
    char* in_string = new char[FILE_NAME_LENGTH];
    for (unsigned long c = 0; c < symlink_count; c++)
    {
        UngeneratedFileEntry mine = r->files_to_gen.at(symlink_ids[c]);
        unsigned long arr_id = file_map[mine.file_name];

        strncpy(r->files[arr_id].name, mine.file_name.substr(0, FILE_NAME_LENGTH-1).c_str(), FILE_NAME_LENGTH-1);
        r->files[arr_id].name[FILE_NAME_LENGTH-1] = 0x00;
        r->files[arr_id].directory_index = dir_map[mine.relative_dir];
        r->files[arr_id].physical_data_pointer = 0xFFFFFFFF;
        r->files[arr_id].file_size = 0x0;
        if (!r->files[arr_id].static_file)
        {
            r->files[arr_id].unique_id = curr_unique | 0xFF000000;
        }
        else
        {
            r->files[arr_id].unique_id = curr_unique;
        }
        curr_unique++;
        r->files[arr_id].file_physical_data = nullptr;
        r->files[arr_id].crc32 = 0xFFFFFFFF;
        r->files[arr_id].physical_file_size = 0x0;

        file_in.open(mine.file_path, std::ios::in | std::ios::binary);
        if (file_in.bad() || !file_in.is_open())
        {
            printf("Cannot load file: %s\n", mine.file_path.c_str());
            unloadRKV(r);
            return false;
        }

        file_in.read(in_string, FILE_NAME_LENGTH);
        in_string[FILE_NAME_LENGTH-1] = 0x00;
        std::string in_string_conv(in_string);

        if (file_map.contains(in_string_conv))
        {
            id_of_link = file_map[in_string_conv];
            r->files[arr_id].timestamp = r->files[id_of_link].timestamp;
        }
        else
        {
            printf("No corresponding file for symlink '%s', using -1 timestamp\n", mine.file_name.c_str());
            r->files[arr_id].timestamp = -1;
        }
    }
    delete[] in_string;

    //calculate last_loc
    unsigned long long last_loc = 0;
    unsigned long long rounded_loc = 0;
    for (unsigned long p_id = 0; p_id < r->file_count; p_id++)
    {
        if (r->files[p_id].physical_data_pointer == 0xFFFFFFFF) { continue; }
        else if (r->files[p_id].physical_data_pointer + r->files[p_id].file_size > last_loc)
        {
            last_loc = r->files[p_id].physical_data_pointer + r->files[p_id].file_size;
            rounded_loc = r->files[p_id].physical_data_pointer + r->files[p_id].physical_file_size;
        }
    }

    last_loc = getRoundedPointer(last_loc);

    r->data_size += last_loc - rounded_loc;
    r->data_size += 0x08;
    r->physical_data = (char*)calloc(r->data_size, 1);

    for (unsigned long p_id = 0; p_id < r->file_count; p_id++)
    {
        if (r->files[p_id].physical_data_pointer == 0xFFFFFFFF) { continue; }
        /*if (r->files[p_id].directory_index == r->pp_dir_ind)
        {
            printf("'%s': Adding 0x%llX bytes at 0x%lX\n", r->files[p_id].name, r->files[p_id].file_size, r->files[p_id].physical_data_pointer);
        }*/
        memcpy((void*)(r->physical_data + r->files[p_id].physical_data_pointer), (void*)r->files[p_id].file_physical_data, r->files[p_id].file_size);
        delete[] r->files[p_id].file_physical_data;
    }

    for (unsigned long f_id = 0; f_id < r->file_count; f_id++)
    {
        memcpy((void*)(r->physical_data + last_loc), r->files[f_id].name, FILE_NAME_LENGTH);

        r->physical_data[last_loc + FILE_NAME_LENGTH] = r->files[f_id].directory_index & 0xFF;
        r->physical_data[last_loc + FILE_NAME_LENGTH + 0x1] = (r->files[f_id].directory_index & 0xFF00) >> 8;
        r->physical_data[last_loc + FILE_NAME_LENGTH + 0x2] = (r->files[f_id].directory_index & 0xFF0000) >> 16;
        r->physical_data[last_loc + FILE_NAME_LENGTH + 0x3] = (r->files[f_id].directory_index & 0xFF000000) >> 24;

        r->physical_data[last_loc + FILE_NAME_LENGTH + 0x04] = (unsigned long long)(r->files[f_id].file_size & 0xFF);
        r->physical_data[last_loc + FILE_NAME_LENGTH + 0x05] = (unsigned long long)(r->files[f_id].file_size & 0xFF00) >> 8;
        r->physical_data[last_loc + FILE_NAME_LENGTH + 0x06] = (unsigned long long)(r->files[f_id].file_size & 0xFF0000) >> 16;
        r->physical_data[last_loc + FILE_NAME_LENGTH + 0x07] = (unsigned long long)(r->files[f_id].file_size & 0xFF000000) >> 24;
        r->physical_data[last_loc + FILE_NAME_LENGTH + 0x08] = (unsigned long long)(r->files[f_id].file_size & 0xFF00000000) >> 32;
        r->physical_data[last_loc + FILE_NAME_LENGTH + 0x09] = (unsigned long long)(r->files[f_id].file_size & 0xFF0000000000) >> 40;
        r->physical_data[last_loc + FILE_NAME_LENGTH + 0x0A] = (unsigned long long)(r->files[f_id].file_size & 0xFF000000000000) >> 48;
        r->physical_data[last_loc + FILE_NAME_LENGTH + 0x0B] = (unsigned long long)(r->files[f_id].file_size & 0xFF00000000000000) >> 56;

        r->physical_data[last_loc + FILE_NAME_LENGTH + 0x0C] = r->files[f_id].physical_data_pointer & 0xFF;
        r->physical_data[last_loc + FILE_NAME_LENGTH + 0x0D] = (r->files[f_id].physical_data_pointer & 0xFF00) >> 8;
        r->physical_data[last_loc + FILE_NAME_LENGTH + 0x0E] = (r->files[f_id].physical_data_pointer & 0xFF0000) >> 16;
        r->physical_data[last_loc + FILE_NAME_LENGTH + 0x0F] = (r->files[f_id].physical_data_pointer & 0xFF000000) >> 24;

        r->physical_data[last_loc + FILE_NAME_LENGTH + 0x10] = r->files[f_id].crc32 & 0xFF;
        r->physical_data[last_loc + FILE_NAME_LENGTH + 0x11] = (r->files[f_id].crc32 & 0xFF00) >> 8;
        r->physical_data[last_loc + FILE_NAME_LENGTH + 0x12] = (r->files[f_id].crc32 & 0xFF0000) >> 16;
        r->physical_data[last_loc + FILE_NAME_LENGTH + 0x13] = (r->files[f_id].crc32 & 0xFF000000) >> 24;

        r->physical_data[last_loc + FILE_NAME_LENGTH + 0x14] = r->files[f_id].timestamp & 0xFF;
        r->physical_data[last_loc + FILE_NAME_LENGTH + 0x15] = (r->files[f_id].timestamp & 0xFF00) >> 8;
        r->physical_data[last_loc + FILE_NAME_LENGTH + 0x16] = (r->files[f_id].timestamp & 0xFF0000) >> 16;
        r->physical_data[last_loc + FILE_NAME_LENGTH + 0x17] = (r->files[f_id].timestamp & 0xFF000000) >> 24;

        r->physical_data[last_loc + FILE_NAME_LENGTH + 0x18] = (r->files[f_id].unique_id & 0xFF000000) >> 24;
        r->physical_data[last_loc + FILE_NAME_LENGTH + 0x19] = (r->files[f_id].unique_id & 0xFF0000) >> 16;
        r->physical_data[last_loc + FILE_NAME_LENGTH + 0x1A] = (r->files[f_id].unique_id & 0xFF00) >> 8;
        r->physical_data[last_loc + FILE_NAME_LENGTH + 0x1B] = r->files[f_id].unique_id & 0xFF;

        last_loc += FILE_ENTRY_PHYSICAL_SIZE;
    }

    for (unsigned long d_id = 0; d_id < r->directory_count; d_id++)
    {
        memcpy((void*)(r->physical_data + last_loc), (void*)(r->directories[d_id].name), DIRECTORY_ENTRY_PHYSICAL_SIZE);
        last_loc += DIRECTORY_ENTRY_PHYSICAL_SIZE;
    }

    r->physical_data[last_loc] = r->file_count & 0xFF;
    r->physical_data[last_loc + 0x1] = (r->file_count & 0xFF00) >> 8;
    r->physical_data[last_loc + 0x2] = (r->file_count & 0xFF0000) >> 16;
    r->physical_data[last_loc + 0x3] = (r->file_count & 0xFF000000) >> 24;

    r->physical_data[last_loc + 0x4] = r->directory_count & 0xFF;
    r->physical_data[last_loc + 0x5] = (r->directory_count & 0xFF00) >> 8;
    r->physical_data[last_loc + 0x6] = (r->directory_count & 0xFF0000) >> 16;
    r->physical_data[last_loc + 0x7] = (r->directory_count & 0xFF000000) >> 24;

    r->generated = true;
    delete[] symlink_ids;
    return true;
}

bool saveRKV(RKVFile* r, std::string file_out)
{
    //save the generated rkv data to a file
    if (!r->generated || !r->loaded)
    {
        return false;
    }

    std::fstream rkv_out;

    rkv_out.open(file_out, std::ios::out | std::ios::binary);
    if (rkv_out.bad() || !rkv_out.is_open())
    {
        printf("Cannot open file to save: %s\n", file_out.c_str());
        return false;
    }

    printf("Data Size while Writing: 0x%llX\n", r->data_size);

    rkv_out.write(r->physical_data, r->data_size);
    rkv_out.close();

    return true;
}

bool getCommandInput(std::string*& input, unsigned char& argCount)
{
    for (unsigned char i = 0; i < argCount; i++)
    {
        input[i].clear();
    }
    argCount = 0;

    printf("> ");

    char* str = new char[INPUT_CHARACTER_MAX * ARG_COUNT_MAX];
    fgets(str, INPUT_CHARACTER_MAX * ARG_COUNT_MAX, stdin);

    unsigned int loc = 0;
    unsigned int currLen = 0;
    char c;
    bool inQuote = false;
    bool done = false;

    while (!done)
    {
        if (argCount >= ARG_COUNT_MAX) { done = true; break; }

        c = str[loc];
        if (c == '\"')
        {
            if (inQuote)
            {
                inQuote = false;
                argCount++;
                currLen = 0;
            }
            else
            {
                inQuote = true;
            }
        }
        else if (c == ' ')
        {
            if (currLen != 0)
            {
                if (inQuote)
                {
                    input[argCount] += c;
                    currLen++;
                }
                else
                {
                    argCount++;
                    currLen = 0;
                }
            }
        }
        else if (c == 0x00 || c == '\n')
        {
            done = true;
            if (currLen > 0) { argCount++; }
        }
        else
        {
            input[argCount] += c;
            currLen++;
        }

        loc++;
    }

    fflush(stdin);

    delete[] str;

    return !inQuote;
}

void printBool(bool b)
{
    printf("%s\n", b == true ? "true" : "false");
    return;
}

int main (int argc, char* argv[])
{
    std::string* args = new std::string[ARG_COUNT_MAX];
    unsigned char argCount = 0;

    RKVFile in;
    RKVFile out;

    bool quit = false;
    bool response = false;
    while (!quit)
    {
        getCommandInput(args, argCount);

        /*for (int i = 0; i < argCount; i++)
        {
            printf("Arg #%i: %s\n", i, args[i].c_str());
        }*/

        if (argCount > 0)
        {
            //process the args in here
            if (args[0] == "quit" || args[0] == "exit")
            {
                unloadRKV(&in);
                unloadRKV(&out);
                quit = true;
            }
            else if (args[0] == "load")
            {
                if (argCount != 3)
                {
                    printf("Incorrect Args\n");
                }
                else
                {
                    if (args[2] == "in")
                    {
                        response = loadRKV(&in, args[1]);
                        printBool(response);
                    }
                    else if (args[2] == "out")
                    {
                        response = loadRKV(&out, args[1]);
                        printBool(response);
                    }
                    else
                    {
                        printf("Incorrect Args\n");
                    }
                }
            }
            else if (args[0] == "unload")
            {
                if (argCount != 2)
                {
                    printf("Incorrect Args\n");
                }
                else
                {
                    if (args[1] == "in")
                    {
                        response = unloadRKV(&in);
                        printBool(response);
                    }
                    else if (args[1] == "out")
                    {
                        response = unloadRKV(&out);
                        printBool(response);
                    }
                    else
                    {
                        printf("Incorrect Args\n");
                    }
                }
            }
            else if (args[0] == "poke")
            {
                if (argCount != 4)
                {
                    printf("Incorrect Args\n");
                }
                else
                {
                    if (args[1] == "in")
                    {
                        if (args[2] == "file")
                        {
                            response = pokeFile(&in, std::stol(args[3]));
                            printBool(response);
                        }
                        else if (args[2] == "dir")
                        {
                            response = pokeDir(&in, std::stol(args[3]));
                            printBool(response);
                        }
                        else
                        {
                            printf("Incorrect Args\n");
                        }
                    }
                    else if (args[1] == "out")
                    {
                        if (args[2] == "file")
                        {
                            response = pokeFile(&out, std::stol(args[3]));
                            printBool(response);
                        }
                        else if (args[2] == "dir")
                        {
                            response = pokeDir(&out, std::stol(args[3]));
                            printBool(response);
                        }
                        else
                        {
                            printf("Incorrect Args\n");
                        }
                    }
                    else
                    {
                        printf("Incorrect Args\n");
                    }
                }
            }
            else if (args[0] == "extract")
            {
                if (!(argCount >= 4))
                {
                    printf("Incorrect Args\n");
                }
                else if (args[1] == "in")
                {
                    if (args[2] == "*")
                    {
                        response = extractRKV(&in, args[3], -1, args[4] == "false" ? false : true);
                        printBool(response);
                    }
                    else
                    {
                        response = extractRKV(&in, args[3], std::stol(args[2]), args[4] == "false" ? false : true);
                        printBool(response);
                    }
                }
                else if (args[1] == "out")
                {
                    if (args[2] == "*")
                    {
                        response = extractRKV(&out, args[3], -1, args[4] == "false" ? false : true);
                        printBool(response);
                    }
                    else
                    {
                        response = extractRKV(&out, args[3], std::stol(args[2]), args[4] == "false" ? false : true);
                        printBool(response);
                    }
                }
            }
            else if (args[0] == "load_dir")
            {
                if (!(argCount >= 4))
                {
                    printf("Incorrect Args\n");
                }
                else if (args[3] == "in")
                {
                    response = loadDirectoryIntoRKV(&in, args[1], args[2], args[4] == "false" ? false : true);
                    printBool(response);
                }
                else if (args[3] == "out")
                {
                    response = loadDirectoryIntoRKV(&out, args[1], args[2], args[4] == "false" ? false : true);
                    printBool(response);
                }
            }
            else if (args[0] == "generate")
            {
                if (argCount != 2)
                {
                    printf("Incorrect Args\n");
                }
                else if (args[1] == "in")
                {
                    response = generateRKV(&in);
                    printBool(response);
                }
                else if (args[1] == "out")
                {
                    response = generateRKV(&out);
                    printBool(response);
                }
            }
            else if (args[0] == "save")
            {
                if (argCount != 3)
                {
                    printf ("Incorrect Args\n");
                }
                else if (args[1] == "in")
                {
                    response = saveRKV(&in, args[2]);
                    printBool(response);
                }
                else if (args[1] == "out")
                {
                    response = saveRKV(&out, args[2]);
                    printBool(response);
                }
            }
        }
        else
        {
            printf("No Command Inputted\n");
        }
    }
    return 0;
}
