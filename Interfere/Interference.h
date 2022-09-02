#pragma once

#include <BMLPlus/BMLAll.h>
#include <map>

#define BALL_WOOD 1
#define BALL_PAPER 2
#define BALL_STONE 3

typedef const char* C_CKSTRING;

extern "C" {
	__declspec(dllexport) IMod* BMLEntry(IBML* bml);
}

class Interference : public IMod {
public:
	Interference(IBML* bml) : IMod(bml) {}

	virtual C_CKSTRING GetID() override { return "Interference"; }
	virtual C_CKSTRING GetVersion() override { return BML_VERSION; }
	virtual C_CKSTRING GetName() override { return "Interference"; }
	virtual C_CKSTRING GetAuthor() override { return "Ghomist"; }
	virtual C_CKSTRING GetDescription() override { return "Just another mod to drive you crazy..."; }
	DECLARE_BML_VERSION;
	virtual void OnLoad();
	virtual void OnProcess();
	virtual void OnStartLevel();
	virtual void OnPostExitLevel();
	virtual void OnModifyConfig(CKSTRING category, CKSTRING key, IProperty* prop);

private:
	CK3dObject* GetPlayerBall();
	int GetPlayerBallType(CK3dObject* player);
	void UpdateAllShadow();
	void RenderAllShadow();
	void ClearAllShadow();

private:
	CKDataArray* current_level;
	CKRenderContext* _render;

	std::vector<VxVector> vector_list = {};
	std::vector<CK3dObject*> wood_list = {};
	std::vector<CK3dObject*> stone_list = {};
	std::vector<CK3dObject*> paper_list = {};
	int _size_ = 0;

	bool _enable_ = false;

	IProperty* shadow_enable;
	IProperty* distance;
	IProperty* transparency;

	IProperty* positive_x;
	IProperty* positive_z;
	IProperty* negtive_x;
	IProperty* negtive_z;
};