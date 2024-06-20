#pragma once
#include <stdint.h>
#include <vector>
#include <Vector3.h>
#include <string>

enum class MapChipType {
	kBlank,  // 空白
	kBlock,  // ブロック
};

struct MapChipData {
	std::vector<std::vector<MapChipType>> data;
};


/// <summary>
/// マップチップフィールド
/// </summary>
class MapChipField {
public:
	struct IndexSet {
		uint32_t xIndex;
		uint32_t yIndex;
	};
	// 範囲矩形
	struct Rect {
		float left;     // 左端
		float right;    // 右端
		float bottom;   // 下端
		float top;      // 上端
	};

	void ResetMapChipData();
	void LoadMapChipCsv(const std::string& filePath);

	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);
	Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);
	static inline const uint32_t GetNumBlockVirtical() { return kNumBlockVirtical; }
	static inline const uint32_t GetNumBlockHorizontal() { return kNumBlockHorizontal; }
	IndexSet GetMapChipIndexSetByPosition(const Vector3& position);
	Rect GetRectByIndex(uint32_t xIndex, uint32_t yIndex);

private:
	// 1ブロックのサイズ
	static inline const float kBlockWidth = 1.0f;
	static inline const float kBlockHeight = 1.0f;
	// ブロックの個数
	static inline const uint32_t kNumBlockVirtical = 20;
	static inline const uint32_t kNumBlockHorizontal = 100;

	MapChipData mapChipData_;
};
