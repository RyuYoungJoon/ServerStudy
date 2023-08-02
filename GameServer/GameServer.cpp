#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <vector>
#include <mutex>
#include <windows.h>
#include <future>
#include "ThreadManager.h"

#include "RefCounting.h"
#include "Memory.h"
#include "Allocator.h"

using TL = TypeList<class Player, class Mage, class Knight, class Archer>;

enum PLAYER_TYPE
{
	KNIGHT,
	MAGE,
};

class Player
{
public:
	Player()
	{
		INIT_TL(Player);
	}
	virtual ~Player(){}
	DECLARE_TL;

};


class Knight : public Player
{
public:
	Knight() { INIT_TL(Knight); }
};

class Mage : public Player
{
public:
	Mage() { INIT_TL(Mage); }
};

class Archer : public Player
{
public:
	Archer() { INIT_TL(Archer); }
};



int main()
{
	/*TypeList<Mage, Knight>::Head whoAmI;
	TypeList<Mage, Knight>::Tail whoAmI2;

	TypeList<Mage, TypeList<Knight, Archer>>::Head whoAmI3;
	TypeList<Mage, TypeList<Knight, Archer>>::Tail::Head whoAmI4;
	TypeList<Mage, TypeList<Knight, Archer>>::Tail::Tail whoAmI5;

	int len1 = Length<TypeList<Mage, Knight>>::value;
	int len2 = Length<TypeList<Mage, Knight, Archer>>::value;
	
	using TL = TypeList<Player, Mage, Knight, Archer>;
	TypeAt<TL, 0>::Result whoAmI6;
	TypeAt<TL, 1>::Result whoAmI7;
	TypeAt<TL, 2>::Result whoAmI8;

	int index1 = IndexOf<TL, Mage>::value;

	bool canConvert1 = Conversion<Player, Knight>::exists;
	bool canConvert2 = Conversion<Knight, Player>::exists;
	bool canConvert3 = Conversion<Knight, Mage>::exists;*/

	/*{
		Player* player = new Knight();

		bool canCast = CanCast<Knight*>(player);
		Knight* knight = TypeCast<Knight*>(player);

		delete player;
	}*/

	{
		shared_ptr<Player> player = MakeShared<Knight>();

		shared_ptr<Player> archer = TypeCast<Archer>(player);
		bool canCast = CanCast<Mage>(player);
	}

	for (int32 i = 0; i < 5; ++i)
	{
		GThreadManger->Launch([]()
			{
				while (true)
				{

				}
			});
	}


	GThreadManger->Join();
}
