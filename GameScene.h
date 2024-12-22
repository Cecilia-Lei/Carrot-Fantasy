#ifndef _Game_Scene_H_
#define _Game_Scene_H_



#include"cocos2d.h"
#include"Monster.h"
#include"Turret/Turret.h"
#include"Turret/Bullet.h"
#include"Data/PointDelegate.h"
#include"Data/TurretData.h"
#include"Data/MonsterData.h"
#include"json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include"Network/server.h"
#include"Network/client.h"


USING_NS_CC;

// ��Ϸ�߼�
class GameScene :public Layer
{
protected:
	Size screenSize;           // ��ȡ��Ļsize 
	float _screenWidth, _screenHeight;  //��Ļ����

	// ��ʼ������
	int _currNum = 0;            // ��ǰ���ﲨ��
	int _goldValue = 200;          // ��ҵ�ǰ�������
	Sprite* _carrot;          // �ܲ�
	int carrotHealth = 5;     // ֱ������Ӱɣ��ܲ�������ֵ

	// ��json�ж�ȡ
	std::string _tileFile;   // �ؿ���ͼ���ƣ��ļ�·��
	TMXTiledMap* _tileMap;   // ��ͼ
	TMXLayer* _collidable;   // �ϰ��㣬���ɽ����λ��
	int _monsterAll;                    // �ؿ�����������
	int _monsterWave;                     // ���ﲨ��
	std::vector<int> _everyWave;                  // ÿ����������
	Vector<MonsterData*> _monsterDatas;   // ��ǰ�ؿ����й�����Ϣ
	Vector<TurretData*> _turretDatas;     // ��ǰ�ؿ���̨��Ϣ
	Vector<PointDelegate*> _pathPoints;   // ��¼��Ч·����

	// ��Ϸ��Ϣ��¼
	int isTurretAble[15][10];               // �ɽ�����̨��λ�õ�ͼ�����Ҽ�¼λ������������ȼ�
	Vector<Monster*> _currentMonsters;       // �����ִ�Ĺ���
	Vector<Turret*> _currentTurrets;         // �����ִ������
	int _monsterDeath = 0;                   // ���ݻٵĹ�������������ĺ͵��յ��
	int _isPaused = 0;                       // ��ǵ�ǰ��Ϸ�Ƿ���ͣ
	int _isFinalWave = 0;                   // ��ǵ�ǰ��Ϸ�Ƿ������һ������
	int _carrotTag = 0;                     // ����ܲ�λ�õ�tag
	int _carrotCost = 150;                  //��ǵ���ܲ���Ѫ�Ļ���


	// ������ǩ
	Label* _numberLabel;                  // ��ʾ���ﲨ��
	Label* _curNumberLabel;               // ��ʾ��ǰ���ﲨ��
	Label* _goldLabel;                    // ��ʾ��ǰ��ҽ��

	//��Ϸ�ڹ���ʵ����Ҫ
	EventMouse* buildEvent;           //��¼����������λ�õĵ���¼�
	int hasBuild = 0;                 //��¼�Ƿ��Ѿ������������
	int hasUpgrade = 0;               //��¼�Ƿ��Ѿ������������۽���
	Layer* touchLayer;				//���ڽ����������۽���Ĳ�
	EventListenerMouse* touchListener;//���ڽ����������۽���ļ�����

	// ���ڱ�����Ϸ����
	int _monsterNum = 0;             // �Ѿ����ɵĹ�������
	Vector<MonsterData*> _monsterSaveDatas;   // ��ǰ�ؿ��浵������Ϣ
	std::string gameMassageBuffer; // ���ڴ浵��json
	// ���������
	UDPServer udpserver;
	std::mutex serverMutex; // ������


public:
	//
	//~GameScene();
	// ���ݹؿ���Ŵ�����Ϸ�ؿ�����
	static Scene* createSceneWithLevel(int selectLevel, int isSave);
	// �ؿ�������ʼ��
	virtual bool init();
	// ��ȡjson�ؿ�����
	virtual void LoadLevelData();
	// �ؿ���ʼ�������ص�ͼ���ܲ� ����ʼ���ɼ���������
	virtual void initLevel();
	// ��Ļ������ǩ
	virtual void TopLabel();
	// ��Ϸ�еĲ˵�ѡ��
	void onMenuButton();
	// ��Ϸ�е���ͣ��ťѡ��
	void onPauseButton(Ref* pSender);
	// ��Ϸ����
	void gameOver(int isWin);

	// ��ʼʱ����ʱ��Ҳ��������ͣ�����¿�ʼ
	void CountDown();

	// ������¼�,����ѡ�񴴽���̨
	void onMouseDown(EventMouse* event);

	//����յ��¼�
	void TouchLand(EventMouse* event);
	//�������¼�
	void BuildTower(EventMouse* event, int numTower);
	//��������¼�
	void TouchTower(EventMouse* event);
	//���������¼�
	void UpgradeTower(EventMouse* event);
	//���������¼�
	void SaleTower(EventMouse* event);

	// TMX point ->Screen
	// ��ͼ��������ת������Ļ����

	Vec2 TMXPosToLocation(Vec2 pos);
	// Screen ->TMX point
	// ��Ļ����ת���ɵ�ͼ��������	
	Vec2 LocationToTMXPos(Vec2 pos);
	// ���ɹ���
	void generateMonsters();
	void generateMonsterWave();

	//getCarrotHealth
	int getCarrotHealth() {
		return carrotHealth;
	}

	// �й��ﵽ���յ㣬���ܲ�����˺������ж���Ϸ�Ƿ�ʧ��
	void HurtCarrot(int isHurt);

	// ��������ɾ�����Ϲ���
	void removeMonster(Monster* monster);

	// ����ִ����
	Vector<Monster*>& getMonsters();

	// ��Ϸ����ÿ֡����
	virtual void update(float dt) override;

	// ���¹���ж��Ƿ����������ӽ�ң����ӱ�ǻ򵽴��յ㣬���ܲ�����˺�
	void updateMonster();
	// ������Ϸ״̬�����µ�ǰ��ҽ�ұ�ǩ���ж���Ϸ�Ƿ�������ɹ���ʧ��
	void updateGameState();

	void setBuildEvent(EventMouse* event) {
		buildEvent = event;
	}

	EventMouse* getBuildEvent() {
		return buildEvent;
	}

	void setHasBuild(int b) {
		hasBuild = b;
	}

	int getHasBuild() {
		return hasBuild;
	}

	void setHasUpgrade(int b) {
		hasUpgrade = b;
	}

	int getHasUpgrade() {
		return hasUpgrade;
	}


	void createTouchLayer() {
		touchLayer = Layer::create();
	}

	void removeTouchLayer() {
		touchLayer = NULL;
	}

	void setTouchLayer(Layer* ptr) {
		touchLayer = ptr;
	}

	Layer* getTouchLayer() {
		return touchLayer;
	}

	void createTouchListener() {
		touchListener = EventListenerMouse::create();
	}

	void removeTouchListener() {
		touchListener = NULL;
	}

	void setTouchListener(EventListenerMouse* ptr) {
		touchListener = ptr;
	}

	EventListenerMouse* getTouchListener() {
		return touchListener;
	}
	// ��ͣ
	void setIsPaused(int option) {
		_isPaused = option;
	}
	int getIsPaused() {
		return _isPaused;
	}
	// ���һ��
	void setIsFinalWave(int option) {
		_isFinalWave = option;
	}
	int getIsFinalWave() {
		return _isFinalWave;
	}
	// ��ȡ�ܲ���tag
	void setCarrotTag(int option) {
		_carrotTag = option;
	}
	int getCarrotTag() {
		return _carrotTag;
	}
	// �浵
	void SaveGame();
	// ��ȡ�浵
	void LoadSaveGame();
	// ��ȡ�浵��ĳ�ʼ��
	void initSaveGame();
	// ��ʼ�浵��Ϸ
	void beganSaveGame();

	// ��������
	void startServer();
	//
	void onExit() {
		udpserver.Stop();
		Layer::onExit();
	}

	CREATE_FUNC(GameScene);
};

#endif // !_Game_Scene_H