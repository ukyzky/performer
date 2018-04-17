#include "UserScale.h"

UserScale::Array UserScale::userScales = {{ "USER1", "USER2", "USER3", "USER4" }};

UserScale::UserScale(const char *name) :
    Scale(name)
{
    clear();
}

UserScale::UserScale() :
    UserScale("User")
{
}

void UserScale::clear() {
    StringUtils::copy(_userName, "INIT", sizeof(_userName));
    setMode(Mode::Note);
    clearItems();
}

void UserScale::clearItems() {
    setSize(1);
    _items.fill(0);
    if (_mode == Mode::Voltage) {
        _items[1] = 1000;
    }
}

void UserScale::write(WriteContext &context) const {
    auto &writer = context.writer;
    writer.write(_mode);
    writer.write(_size);
    for (int i = 0; i < _size; ++i) {
        writer.write(_items[i]);
    }
}

void UserScale::read(ReadContext &context) {
    clear();

    auto &reader = context.reader;
    reader.read(_mode);
    reader.read(_size);
    for (int i = 0; i < _size; ++i) {
        reader.read(_items[i]);
    }
}

fs::Error UserScale::write(const char *path) const {
    fs::FileWriter fileWriter(path);
    if (fileWriter.error() != fs::OK) {
        fileWriter.error();
    }

    Writer writer(fileWriter);
    FileHeader header(FileType::UserScale, 0, _userName);
    writer.write(&header, sizeof(header));

    WriteContext context = { writer };
    write(context);

    return fileWriter.finish();
}

fs::Error UserScale::read(const char *path) {
    fs::FileReader fileReader(path);
    if (fileReader.error() != fs::OK) {
        fileReader.error();
    }

    Reader reader(fileReader);

    FileHeader header;
    reader.read(&header, sizeof(header));

    header.readName(_userName, sizeof(_userName));

    ReadContext context = { reader };
    read(context);

    return fileReader.finish();
}
