#include "Interference.h"

IMod* BMLEntry(IBML* bml) {
	return new Interference(bml);
}

void Interference::OnLoad() {
	_render = m_bml->GetRenderContext();

	GetConfig()->SetCategoryComment("ShadowBall", "Shadow ball interference.");
	shadow_enable = GetConfig()->GetProperty("ShadowBall", "Enable");
	shadow_enable->SetComment("Enable some shadow ball");
	shadow_enable->SetDefaultBoolean(false);
	_enable_ = shadow_enable->GetBoolean();

	distance = GetConfig()->GetProperty("ShadowBall", "Distance");
	distance->SetComment("Gap distance.");
	distance->SetDefaultFloat(5.0f);

	transparency = GetConfig()->GetProperty("ShadowBall", "Transparency");
	transparency->SetComment("Transparency of shadow balls. Default: 0.5");
	transparency->SetDefaultFloat(0.5f);

	positive_x = GetConfig()->GetProperty("ShadowBall", "X+");
	positive_x->SetComment("Count of shadow balls at 'x+' axis.");
	positive_x->SetDefaultInteger(1);

	negtive_x = GetConfig()->GetProperty("ShadowBall", "X-");
	negtive_x->SetComment("Count of shadow balls at 'x-' axis.");
	negtive_x->SetDefaultInteger(1);

	positive_z = GetConfig()->GetProperty("ShadowBall", "Z+");
	positive_z->SetComment("Count of shadow balls at 'z+' axis.");
	positive_z->SetDefaultInteger(1);

	negtive_z = GetConfig()->GetProperty("ShadowBall", "Z-");
	negtive_z->SetComment("Count of shadow balls at 'z-' axis.");
	negtive_z->SetDefaultInteger(1);
}

CK3dObject* Interference::GetPlayerBall() {
	return (CK3dObject*)current_level->GetElementObject(0, 1);
}

int Interference::GetPlayerBallType(CK3dObject* player) {
	std::string name = player->GetName();
	if (name == "Ball_Wood")
		return BALL_WOOD;
	else if (name == "Ball_Paper")
		return BALL_PAPER;
	else
		return BALL_STONE;
}

void Interference::OnStartLevel() {
	current_level = m_bml->GetArrayByName("CurrentLevel");
	UpdateAllShadow();
}

void Interference::OnProcess() {
	if (m_bml->IsPlaying() && _enable_) {
		RenderAllShadow();
	}
}

void Interference::OnPostExitLevel() {
	ClearAllShadow();
}

void Interference::OnModifyConfig(CKSTRING category, CKSTRING key, IProperty* prop) {
	_enable_ = shadow_enable->GetBoolean();
	if (m_bml->IsIngame()) {
		UpdateAllShadow();
		RenderAllShadow();
	}
}

void Interference::UpdateAllShadow() {
	ClearAllShadow();

	CKContext* ctx = m_bml->GetCKContext();

	CK3dObject* _wood = m_bml->Get3dObjectByName("Ball_Wood");
	CK3dObject* _stone = m_bml->Get3dObjectByName("Ball_Stone");
	CK3dObject* _paper = m_bml->Get3dObjectByName("Ball_Paper");

	float gap = distance->GetFloat();
	CK3dObject* new_ball;
	char name[50] = "";

	CKDependencies dep;
	dep.Resize(40); dep.Fill(0);
	dep.m_Flags = CK_DEPENDENCIES_CUSTOM;
	dep[CKCID_OBJECT] = CK_DEPENDENCIES_COPY_OBJECT_NAME | CK_DEPENDENCIES_COPY_OBJECT_UNIQUENAME;
	dep[CKCID_MESH] = CK_DEPENDENCIES_COPY_MESH_MATERIAL;
	dep[CKCID_3DENTITY] = CK_DEPENDENCIES_COPY_3DENTITY_MESH;

	for (int i = 1; i <= positive_x->GetInteger(); ++i) {
		vector_list.emplace_back(VxVector(gap * i, 0.0f, 0.0f));

		new_ball = (CK3dObject*)ctx->CopyObject(_wood, &dep);
		sprintf(name, "Ball_Wood_x+_%d", i);
		new_ball->SetName(name);
		wood_list.emplace_back(new_ball);

		new_ball = (CK3dObject*)ctx->CopyObject(_stone, &dep);
		sprintf(name, "Ball_Stone_x+_%d", i);
		new_ball->SetName(name);
		stone_list.emplace_back(new_ball);

		new_ball = (CK3dObject*)ctx->CopyObject(_paper, &dep);
		sprintf(name, "Ball_Paper_x+_%d", i);
		new_ball->SetName(name);
		paper_list.emplace_back(new_ball);
	}
	for (int i = 1; i <= positive_z->GetInteger(); ++i) {
		vector_list.emplace_back(VxVector(0.0f, 0.0f, gap * i));

		new_ball = (CK3dObject*)ctx->CopyObject(_wood, &dep);
		sprintf(name, "Ball_Wood_z+_%d", i);
		new_ball->SetName(name);
		wood_list.emplace_back(new_ball);

		new_ball = (CK3dObject*)ctx->CopyObject(_stone, &dep);
		sprintf(name, "Ball_Stone_z+_%d", i);
		new_ball->SetName(name);
		stone_list.emplace_back(new_ball);

		new_ball = (CK3dObject*)ctx->CopyObject(_paper, &dep);
		sprintf(name, "Ball_Paper_z+_%d", i);
		new_ball->SetName(name);
		paper_list.emplace_back(new_ball);
	}
	for (int i = 1; i <= negtive_x->GetInteger(); ++i) {
		vector_list.emplace_back(VxVector(gap * -i, 0.0f, 0.0f));

		new_ball = (CK3dObject*)ctx->CopyObject(_wood, &dep);
		sprintf(name, "Ball_Wood_x-_%d", i);
		new_ball->SetName(name);
		wood_list.emplace_back(new_ball);

		new_ball = (CK3dObject*)ctx->CopyObject(_stone, &dep);
		sprintf(name, "Ball_Stone_x-_%d", i);
		new_ball->SetName(name);
		stone_list.emplace_back(new_ball);

		new_ball = (CK3dObject*)ctx->CopyObject(_paper, &dep);
		sprintf(name, "Ball_Paper_x-_%d", i);
		new_ball->SetName(name);
		paper_list.emplace_back(new_ball);
	}
	for (int i = 1; i <= negtive_z->GetInteger(); ++i) {
		vector_list.emplace_back(VxVector(0.0f, 0.0f, gap * -i));

		new_ball = (CK3dObject*)ctx->CopyObject(_wood, &dep);
		sprintf(name, "Ball_Wood_z-_%d", i);
		new_ball->SetName(name);
		wood_list.emplace_back(new_ball);

		new_ball = (CK3dObject*)ctx->CopyObject(_stone, &dep);
		sprintf(name, "Ball_Stone_z-_%d", i);
		new_ball->SetName(name);
		stone_list.emplace_back(new_ball);

		new_ball = (CK3dObject*)ctx->CopyObject(_paper, &dep);
		sprintf(name, "Ball_Paper_z-_%d", i);
		new_ball->SetName(name);
		paper_list.emplace_back(new_ball);
	}

	float _transpaprency = transparency->GetFloat();
	//std::vector<std::vector<CK3dObject*>> lists = ;
	for (auto list : { wood_list, paper_list, stone_list })
		for (auto ball : list)
			for (int i = 0; i < ball->GetMeshCount(); ++i) {
				CKMesh* mesh = ball->GetMesh(i);
				for (int j = 0; j < mesh->GetMaterialCount(); ++j) {
					CKMaterial* mat = mesh->GetMaterial(j);
					mat->EnableAlphaBlend();
					mat->SetSourceBlend(VXBLEND_SRCALPHA);
					mat->SetDestBlend(VXBLEND_INVSRCALPHA);
					VxColor color = mat->GetDiffuse();
					color.a = _transpaprency;
					mat->SetDiffuse(color);
					//m_bml->SetIC(mat);
				}
			}

	_size_ = positive_x->GetInteger() + negtive_x->GetInteger() + positive_z->GetInteger() + negtive_z->GetInteger();
}

void Interference::RenderAllShadow() {
	CK3dObject* player = GetPlayerBall();

	VxVector player_pos = VxVector();
	VxQuaternion player_qua = VxQuaternion();
	player->GetPosition(&player_pos);
	player->GetQuaternion(&player_qua);

	switch (GetPlayerBallType(player)) {
	case BALL_WOOD:
		for (int i = 0; i < _size_; ++i) {
			stone_list[i]->Show(CKHIDE);
			paper_list[i]->Show(CKHIDE);
			wood_list[i]->Show();
			wood_list[i]->SetPosition(player_pos + vector_list[i]);
			wood_list[i]->SetQuaternion(player_qua);
		}
		break;
	case BALL_PAPER:
		for (int i = 0; i < _size_; ++i) {
			wood_list[i]->Show(CKHIDE);
			stone_list[i]->Show(CKHIDE);
			paper_list[i]->Show();
			paper_list[i]->SetPosition(player_pos + vector_list[i]);
			paper_list[i]->SetQuaternion(player_qua);
		}
		break;
	case BALL_STONE:
		for (int i = 0; i < _size_; ++i) {
			wood_list[i]->Show(CKHIDE);
			paper_list[i]->Show(CKHIDE);
			stone_list[i]->Show();
			stone_list[i]->SetPosition(player_pos + vector_list[i]);
			stone_list[i]->SetQuaternion(player_qua);
		}
		break;
	}
}

void Interference::ClearAllShadow() {
	CKContext* ctx = m_bml->GetCKContext();
	for (auto list : { wood_list, paper_list, stone_list }) {
		for (auto ball : list)
			ctx->DestroyObject(ball);
	}

	wood_list.clear();
	paper_list.clear();
	stone_list.clear();
	vector_list.clear();
	_size_ = 0;
}
