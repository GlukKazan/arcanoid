#include "ResourceManager.h"
#include "Desktop.h"
#include "Locale.h"

ResourceManager rm;
CIwSVec2 xy3[MAX_VERTS];

void ResourceManager::init() {
	IwSoundInit();
	IwResManagerInit();
#ifdef IW_BUILD_RESOURCES
	IwGetResManager()->AddHandler(new CIwResHandlerWAV);
#endif
	IwGetResManager()->LoadGroup("sounds.group");
	imgs = new map<string, SImg>();
	sounds = new map<int, Sound>();
}

void ResourceManager::release() {
	for (IIter p = imgs->begin(); p != imgs->end(); ++p) {
		delete p->second.texture;
		delete p->second.mat;
	}
	delete sounds;
	delete imgs;
	IwResManagerTerminate();
	IwSoundTerminate();
}

void ResourceManager::update() {
    IwGetSoundManager()->Update();
}

void ResourceManager::fireSound(const char* name) {
	if (!checkSound()) return;
    CIwResGroup* resGroup = IwGetResManager()->GetGroupNamed(SOUND_GROUP);
	CIwSoundSpec* SoundSpec = (CIwSoundSpec*)resGroup->GetResNamed(name, IW_SOUND_RESTYPE_SPEC);
	CIwSoundInst* SoundInstance = SoundSpec->Play();
}

void ResourceManager::fireSound(int id) {
	SIter p = sounds->find(id);
	if (p != sounds->end()) {
		fireSound(p->second.nm);
	}
}

void ResourceManager::regSound(int id, const char* name) {
	Sound s;
	s.id = id;
	s.nm = pool.string(name);
	sounds->insert(SPair(id, s));
}

CommonObject* ResourceManager::createContext(const char* name) {
	currSound.clear();
	return &currSound;
}

bool ResourceManager::closeContext(int scope) {
	if (!currSound.isValid()) return false;
	sounds->insert(SPair(currSound.id, currSound));
	return true;
}

CIwMaterial* ResourceManager::load(const char* name, int* w, int* h) {
	int width = 0, height = 0;
	char res[MAX_RES_NAME] = {0};
	sprintf(res, "images/%s/%s", desktop.getDevPath(), name);
	IIter p = imgs->find(res);
	if (p != imgs->end()) {
		if (w != NULL) {
			*w = p->second.width;
		}
		if (h != NULL) {
			*h = p->second.height;
		}
		return p->second.mat;
	}
	CIwTexture* texture = new CIwTexture;
    CIwImage image;
    s3eFile* pFile = s3eFileOpen(res, "rb");
    if (pFile) {
        image.ReadFile(pFile);
		width = image.GetWidth();
		height = image.GetHeight();
        s3eFileClose(pFile);
        texture->CopyFromImage(&image);
        texture->Upload();
    } else {
		delete texture;
		texture = NULL;
	}
	CIwMaterial* mat = new CIwMaterial;
	mat->SetTexture(texture);
	SImg s;
	s.texture = texture;
	s.mat = mat;
	s.width = width;
	s.height = height;
	imgs->insert(IPair(string(res), s));
	if (w != NULL) {
		*w = width;
	}
	if (h != NULL) {
		*h = height;
	}
	return mat;
}

void ResourceManager::clearPaint() {
    vertC = 0;
}

void ResourceManager::paint(const char* res, int x, int y, int w, int h, int* lm, int* tm, int* rm, int* bm) {
	int width, height;
	if (res == NULL) return;
	CIwMaterial* t = load(res, &width, &height);
	if (t == NULL) return;
    if (vertC + 4 >= MAX_VERTS) return;
	if (width != 0) {
		if (lm != 0) {
			*lm = (*lm * w) / width;
			x -= *lm;
			w += *lm;
		}
		if (rm != 0) {
			*rm = (*rm * w) / width;
			w += *rm;
		}
	}
	if (height != 0) {
		if (tm != 0) {
			*tm = (*tm * h) / height;
			y -= *tm;
			h += *tm;
		}
		if (bm != NULL) {
			*bm = (*bm * h) / height;
			h += *bm;
		}
	}
    int offset = vertC;
	xy3[vertC].x = x,     xy3[vertC].y = y;      vertC++;
	xy3[vertC].x = x,     xy3[vertC].y = y + h;  vertC++;
	xy3[vertC].x = x + w, xy3[vertC].y = y + h;  vertC++;
	xy3[vertC].x = x + w, xy3[vertC].y = y;      vertC++;
	IwGxSetVertStreamScreenSpace(xy3 + offset, 4);
	IwGxSetMaterial(t);
	static CIwFVec2 uvs[4] = {
		CIwFVec2(0, 0),
		CIwFVec2(0, 1),
		CIwFVec2(1, 1),
		CIwFVec2(1, 0),
	};
/*	static CIwSVec2 uvs[4] = {
		CIwSVec2(0,           0),
		CIwSVec2(0,           IW_GEOM_ONE),
		CIwSVec2(IW_GEOM_ONE, IW_GEOM_ONE),
		CIwSVec2(IW_GEOM_ONE, 0),
	};*/
	IwGxSetUVStream(uvs);
	IwGxDrawPrims(IW_GX_QUAD_LIST, NULL, 4);
}
