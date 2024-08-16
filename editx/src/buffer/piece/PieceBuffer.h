#pragma once
#include <iostream>
#include <vector>
#include <string>

enum class BufferType{ ORIGINAL, ADD};

struct Piece {
	BufferType bufferType;
	int offset;
	int length;
};

class PieceTable {
private:
	std::string originalBuffer;
	std::string addBuffer;
	std::vector<Piece> pieces;

public:
	void init(const std::string& initialText = "") {
		originalBuffer = initialText;
        pieces.clear();

		pieces.push_back(Piece{ BufferType::ORIGINAL, 0, static_cast<int>(originalBuffer.size()) });
	}

	void insert(int position, const std::string& newText)
	{
		auto addOffset = (int)addBuffer.length();
		addBuffer += newText;

		int pieceIndex = 0;
		int charCount = 0;
		for (; pieceIndex < pieces.size(); pieceIndex++)
		{
			int pieceLength = pieces[pieceIndex].length;
			if (charCount + pieceLength >= position) {
				break;
			}
			charCount += pieceLength;
		}

		Piece& piece = pieces[pieceIndex];
		int relativePos = position - charCount;

		if (relativePos > 0)
		{
			pieces.insert(pieces.begin() + pieceIndex + 1, { piece.bufferType, piece.offset + relativePos, piece.length - relativePos });
			piece.length = relativePos;
		}

		pieces.insert(pieces.begin() + pieceIndex + 1, { BufferType::ADD, addOffset, (int)newText.length() });
	}

    void erase(int position, int length)
    {
        int pieceIndex = 0, charCount = 0;

        for (; pieceIndex < pieces.size(); pieceIndex++)
        {
            int pieceLength = pieces[pieceIndex].length;
            if (charCount + pieceLength > position)
            {
                break;
            }
            charCount += pieceLength;
        }

        int relativePos = position - charCount;
        int remainingLength = length;

        while (remainingLength > 0 && pieceIndex < pieces.size()) {
            Piece& piece = pieces[pieceIndex];

            if (relativePos > 0)
            {
                if (relativePos + remainingLength < piece.length) {
                    pieces.insert(pieces.begin() + pieceIndex + 1, { piece.bufferType, piece.offset + relativePos + remainingLength, piece.length - (relativePos + remainingLength) });
                    piece.length = relativePos;
                    remainingLength = 0;
                }
                else {
                    remainingLength -= (piece.length - relativePos);
                    piece.length = relativePos;
                    relativePos = 0;
                    pieceIndex++;
                }
            }
            else {
                if (remainingLength < piece.length) {
                    piece.offset += remainingLength;
                    piece.length -= remainingLength;
                    remainingLength = 0;
                }
                else {
                    remainingLength -= piece.length;
                    pieces.erase(pieces.begin() + pieceIndex);
                }
            }
        }
    }

    const std::vector<Piece>& getPieces() { return pieces; }

	std::string getText() {
		std::string result;
		for (const Piece& piece : pieces) {
			const std::string& buffer = (piece.bufferType == BufferType::ORIGINAL) ? originalBuffer : addBuffer;
			result += buffer.substr(piece.offset, piece.length);
		}
		return result;
	}
};