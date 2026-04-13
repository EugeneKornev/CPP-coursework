#include <cstddef>
#include <cstdio>
#include <cstring>
#include <stdexcept>
#include <string>

#define READ_FABRIC(type) virtual void read(type) = 0
#define WRITE_FABRIC(type) virtual void write(type) = 0

class IO {
protected:
	bool open;
	bool eof_;

public:
	IO() : open(true), eof_(false) {}

	virtual ~IO() = default;

	bool isOpen() const {
		return open;
	}

	virtual void close() {
		open = false;
	}

	bool eof() const {
		return eof_;
	}
};

class Reader : public virtual IO {
public:
	READ_FABRIC(char& c);
	READ_FABRIC(int& i);
	READ_FABRIC(float& f);
	READ_FABRIC(double& d);
	READ_FABRIC(std::string& s);
};

class Writer : public virtual IO {
public:
	WRITE_FABRIC(char c);
	WRITE_FABRIC(int i);
	WRITE_FABRIC(float f);
	WRITE_FABRIC(double d);
	WRITE_FABRIC(const std::string& s);
};


class ReaderWriter : public Reader, public Writer {};


class StringSource : public virtual IO {
protected:
	std::string* data;
	size_t pos;

public:

	StringSource(std::string& str) : data(&str), pos(0) {}

	StringSource(const std::string& str) : data(const_cast<std::string*>(&str)), pos(0) {}
};


class FileSource : public virtual IO {
protected:
	FILE* file;

public:

	FileSource(FILE* f) : file(f) {
		if (!file) {
			open = false;
		}
	}
};

class StringReader : public virtual Reader, public virtual StringSource {
protected:

	size_t read(void* buffer, size_t size) {
		if (!open) {
			throw std::runtime_error("Source is closed");
		}
		size_t remaining = data->size() - pos;
		size_t bytesToRead = (size < remaining) ? size : remaining;
		if (bytesToRead == 0) {
			eof_ = true;
			return  0;
		}
		std::memcpy(buffer, data->data() + pos, bytesToRead);
		pos += bytesToRead;
		return bytesToRead;
	}

public:

	StringReader(const std::string& str) : StringSource(str) {}

	void read(char& c) override {
		size_t readBytes = read(&c, sizeof(c));
		if (readBytes != sizeof(c)) {
			throw std::runtime_error("EOF");
		}
	}

	void read(int& i) override {
		size_t readBytes = read(&i, sizeof(i));
		if (readBytes != sizeof(i)) {
			throw std::runtime_error("EOF");
		}
	}

	void read(float& f) override {
		size_t readBytes = read(&f, sizeof(f));
		if (readBytes != sizeof(f)) {
			throw std::runtime_error("EOF");
		}
	}

	void read(double& d) override {
		size_t readBytes = read(&d, sizeof(d));
		if (readBytes != sizeof(d)) {
			throw std::runtime_error("EOF");
		}
	}

	void read(std::string& str) override {
		int len;
		read(len);
		str.assign(data->data() + pos, len);
		pos += len;
	}
};


class StringWriter : public virtual Writer, public virtual StringSource {
protected:

	void write(const void* buffer, size_t size) {
		if (!open) {
			throw std::runtime_error("Source is closed");
		}
		const char* bytes = static_cast<const char*>(buffer);
		data->insert(data->end(), bytes, bytes + size);
	}

public:

	StringWriter(std::string& str) : StringSource(str) {}

	void write(char c) override {
		write(&c, sizeof(c));
	}

	void write(int i) override {
		write(&i, sizeof(i)); 
	}

    void write(float f) override {
    	write(&f, sizeof(f));
    }

    void write(double d) override {
    	write(&d, sizeof(d));
    }

    void write(const std::string& str) override {
    	if (!open) {
    		throw std::runtime_error("Source is closed");
    	}
    	write(static_cast<int>(str.size()));
    	write(str.data(), str.size());
    }
};

class StringReaderWriter : public ReaderWriter, public StringReader, public StringWriter {

public:
	StringReaderWriter(std::string& str) : IO(), StringSource(str), StringReader(str), StringWriter(str), ReaderWriter() {}

	void close() override {
		StringReader::close();
		//StringWriter::close();
	}

	using StringReader::read;
	using StringWriter::write;
};


class FileReader : public virtual Reader, public virtual FileSource {
protected:

	size_t read(void* buffer, size_t size) {
		if (!open || !file) {
			throw std::runtime_error("File isn't open");
		}
		size_t readBytes = fread(buffer, 1, size, file);
		if (readBytes < size && feof(file)) {
			eof_ = true;
		}
		return readBytes;
	}

public:

	FileReader(FILE* f) : FileSource(f) {}

	void read(char& c) override {
		read(&c, sizeof(c));
	}

	void read(int& i) override {
		read(&i, sizeof(i));
	}

    void read(float& f) override {
    	read(&f, sizeof(f));
    }

    void read(double& d) override {
    	read(&d, sizeof(d)); 
    }

    void read(std::string& str) override {
    	int len;
    	read(len);
    	str.resize(len);
    	if (len > 0 && fread(&str[0], 1, len, file) != len) {
    		throw std::runtime_error("Couldn't read string");
    	}
    }
};

class FileWriter : public virtual Writer, public virtual FileSource {
protected:

	void write(const void* buffer, size_t size) {
		if (!open || !file) throw std::runtime_error("File isn't open");
        if (fwrite(buffer, 1, size, file) != size)
            throw std::runtime_error("Write error");
	}

public:

	FileWriter(FILE* f) : FileSource(f) {}

	void write(char c) override {
		write(&c, sizeof(c));
	}

	void write(int i) override {
		write(&i, sizeof(i));
	}

    void write(float f) override {
    	write(&f, sizeof(f));
    }

    void write(double d) override {
    	write(&d, sizeof(d));
    }

    void write(const std::string& str) override {
        write(static_cast<int>(str.size()));
        write(str.data(), str.size());
    }
};

class FileReaderWriter : public ReaderWriter, public FileReader, public FileWriter {
public:

	FileReaderWriter(FILE* f) : IO(), FileSource(f), FileReader(f), FileWriter(f), ReaderWriter() {}

	void close() override {
		FileReader::close();
		// FileWriter::close();
	}

	using FileReader::read;
	using FileWriter::write;
};