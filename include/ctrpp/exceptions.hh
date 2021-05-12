#ifndef __exceptions_hh
#define __exceptions_hh

#include <exception>
#include <sstream>
#include <string>

#include <ctrpp/ints.hh>

namespace ctrpp
{
	namespace exceptions
	{
		class BaseException : public std::exception
		{
		public:
			BaseException() { setmsg(); };
			BaseException(const char *msg) { setmsg(msg); };
			BaseException(std::string msg) { setmsg(msg.c_str()); };

			virtual ~BaseException() throw()
			{
			}

			const char *what() throw()
			{
				return this->msg.c_str();
			}

		protected:
			std::string msg;

		private:
			void setmsg(const char *msg = nullptr)
			{
				this->msg = "[ctrpp] ";

				if (msg != nullptr)
				{
					this->msg += msg;
				}
			}
		};

		namespace fs
		{
			class FileException : public BaseException
			{
			public:
				FileException() : BaseException() { setmsg(); }
				FileException(const char *msg) : BaseException() { setmsg(msg); }
				FileException(std::string msg) : BaseException() { setmsg(msg.c_str()); }

			private:
				void setmsg(const char *msg = nullptr)
				{
					this->msg += "[File Error] ";

					if (msg != nullptr)
					{
						this->msg += msg;
					}
				}
			};

			class FileNotFoundException : public FileException
			{
			public:
				FileNotFoundException(const char *filename) : FileException() { setmsg(filename); }
				FileNotFoundException(std::string filename) : FileException() { setmsg(filename.c_str()); }

			private:
				void setmsg(const char *filename)
				{
					std::ostringstream ss;
					ss << "Not Found: \"" << filename << "\"";
					this->msg += ss.str();
				}
			};

			class InvalidFileSizeException : public FileException
			{
			public:
				InvalidFileSizeException(const char *filename, size_t size, size_t expected_size, size_t second_size = 0) : FileException() { setmsg(filename, size, expected_size, second_size); }
				InvalidFileSizeException(std::string filename, size_t size, size_t expected_size, size_t second_size = 0) : FileException() { setmsg(filename.c_str(), size, expected_size, second_size); }

			private:
				void setmsg(const char *filename, size_t size, size_t expected_size, size_t second_size)
				{
					std::ostringstream ss;
					ss << "The file \"" << filename << "\" has an invalid file size (" << size << "), but expected size (" << expected_size << ")";

					if (second_size != 0)
						ss << " or size (" << second_size << ")";

					this->msg += ss.str();
				}
			};

			class PathIsDirectoryException : public FileException
			{
			public:
				PathIsDirectoryException(const char *filename) : FileException() { setmsg(filename); }
				PathIsDirectoryException(std::string filename) : FileException() { setmsg(filename.c_str()); }

			private:
				void setmsg(const char *filename)
				{
					std::ostringstream ss;
					ss << "The path \"" << filename << "\" points to a directory, rather than a file";
					this->msg += ss.str();
				}
			};

			class FileAccessException : public FileException
			{
			public:
				FileAccessException(const char *filename, const char *mode) : FileException() { setmsg(filename, mode); }
				FileAccessException(std::string filename, const char *mode) : FileException() { setmsg(filename.c_str(), mode); }

			private:
				void setmsg(const char *filename, const char *mode)
				{
					std::ostringstream ss;
					ss << "Could not open file \"" << filename << "\" (file mode: \"" << mode << "\")";
					this->msg += ss.str();
				}
			};

			class FileReadException : public FileException
			{
			public:
				FileReadException() : FileException() { setmsg(); }
				FileReadException(const char *filename) : FileException() { setmsg(filename); }
				FileReadException(std::string filename) : FileException() { setmsg(filename.c_str()); }

			private:
				void setmsg(const char *filename)
				{
					std::ostringstream ss;
					ss << "Could not read from opened file \"" << filename << "\"";
					this->msg += ss.str();
				}

				void setmsg()
				{
					this->msg += "Could not read from opened file";
				}
			};

			class FileWriteException : public FileException
			{
			public:
				FileWriteException() : FileException() { setmsg(); }
				FileWriteException(const char *filename) : FileException() { setmsg(filename); }
				FileWriteException(std::string filename) : FileException() { setmsg(filename.c_str()); }

			private:
				void setmsg(const char *filename)
				{
					std::ostringstream ss;
					ss << "Could not write to opened file \"" << filename << "\"";
					this->msg += ss.str();
				}

				void setmsg()
				{
					this->msg += "Could not write to opened file";
				}
			};

			class FileSeekException : public FileException
			{
			public:
				FileSeekException(const char *filename, off_t offset) : FileException() { setmsg(filename, offset); }
				FileSeekException(std::string filename, off_t offset) : FileException() { setmsg(filename.c_str(), offset); }
				FileSeekException(off_t offset) : FileException() { setmsg(offset); }

			private:
				void setmsg(const char *filename, off_t offset)
				{
					std::ostringstream ss;
					ss << "Could not seek opened file \"" << filename << "\" to offset (" << offset << ")";
					this->msg += ss.str();
				}

				void setmsg(off_t offset)
				{
					std::ostringstream ss;
					ss << "Could not seek opened file to offset (" << offset << ")";
					this->msg += ss.str();
				}
			};
		}

		namespace crypto
		{
			class CryptoException : public BaseException
			{
			public:
				CryptoException() : BaseException() { setmsg(); }
				CryptoException(const char *msg) : BaseException() { setmsg(msg); }
				CryptoException(std::string msg) : BaseException() { setmsg(msg.c_str()); }

			private:
				void setmsg(const char *msg = nullptr)
				{
					this->msg += "[Crypto Error] ";

					if (msg != nullptr)
					{
						this->msg += msg;
					}
				}
			};

			class HashMismatchException : public CryptoException
			{
			public:
				HashMismatchException(const char *filename) : CryptoException() { setmsg(filename); }
				HashMismatchException(std::string filename) : CryptoException() { setmsg(filename.c_str()); }

			private:
				void setmsg(const char *filename)
				{
					std::ostringstream ss;
					ss << "Hash mismatch for file at \"" << filename << "\"!";
					this->msg += ss.str();
				}
			};

			class HashInitException : public CryptoException
			{
			public:
				HashInitException(int err_code) : CryptoException()
				{
					std::ostringstream ss;
					ss << "Failed to initialize SHA-256 hashing context (Error Code: " << err_code << ")";
					this->msg += ss.str();
				}
			};

			class HashUpdateException : public CryptoException
			{
			public:
				HashUpdateException(int err_code) : CryptoException()
				{
					std::ostringstream ss;
					ss << "Failed to update SHA-256 hashing context with new data (Error Code: " << err_code << ")";
					this->msg += ss.str();
				}
			};

			class HashFinalizeException : public CryptoException
			{
			public:
				HashFinalizeException(int err_code) : CryptoException()
				{
					std::ostringstream ss;
					ss << "Failed to finalize SHA-256 hashing process (Error Code: " << err_code << ")";
					this->msg += ss.str();
				}
			};
		}

		namespace sig
		{
			class SignatureException : public BaseException
			{
			public:
				SignatureException() : BaseException() { setmsg(); }
				SignatureException(const char *msg) : BaseException() { setmsg(msg); }
				SignatureException(std::string msg) : BaseException() { setmsg(msg.c_str()); }

			private:
				void setmsg(const char *msg = nullptr)
				{
					this->msg += "[Signature Error] ";

					if (msg != nullptr)
					{
						this->msg += msg;
					}
				}
			};
		}

		namespace sig
		{
			class InvalidSignatureTypeException : public BaseException
			{
			public:
				InvalidSignatureTypeException(u32 sig_type) : BaseException() { setmsg(sig_type); }

			private:
				void setmsg(u32 sig_type)
				{
					std::ostringstream ss;
					ss << "[Signature] Invalid signature type (0x" << std::hex << sig_type << ")";
					this->msg += ss.str();
				}
			};
		}

		namespace types
		{
			namespace exefs
			{
				class ExeFSException : public BaseException
				{
				public:
					ExeFSException() : BaseException() { setmsg(); }
					ExeFSException(const char *msg) : BaseException() { setmsg(msg); }
					ExeFSException(std::string msg) : BaseException() { setmsg(msg.c_str()); }

				private:
					void setmsg(const char *msg = nullptr)
					{
						this->msg += "[ExeFS] ";

						if (msg != nullptr)
							this->msg += msg;
					}
				};

				class InvalidEntryNameException : public ExeFSException
				{
				public:
					InvalidEntryNameException(const char *entry_name) : ExeFSException() { setmsg(entry_name); }
					InvalidEntryNameException(std::string entry_name) : ExeFSException() { setmsg(entry_name.c_str()); }

				private:
					void setmsg(const char *entry_name)
					{
						std::ostringstream ss;
						ss << "The entry name \"" << entry_name << " is invalid for an ExeFS";
						this->msg += ss.str();
					}
				};

				class EntryNotFoundException : public ExeFSException
				{
				public:
					EntryNotFoundException(const char *entry_name) : ExeFSException() { setmsg(entry_name); }
					EntryNotFoundException(std::string entry_name) : ExeFSException() { setmsg(entry_name.c_str()); }

				private:
					void setmsg(const char *entry_name)
					{
						std::ostringstream ss;
						ss << "The entry \"" << entry_name << " was not found";
						this->msg += ss.str();
					}
				};
			}
		}
	}
}

#endif