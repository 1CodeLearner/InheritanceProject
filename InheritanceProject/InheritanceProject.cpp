#include <iostream>
#include <string>
#include <random>


class Dice
{
public:
	int Roll(int nDice)
	{
		int total = 0;
		for (int n = 0; n < nDice; n++)
		{
			total += d6(rng);
		}
		return total;
	}
private:
	std::uniform_int_distribution<int> d6 = std::uniform_int_distribution<int>(1, 6);
	std::mt19937 rng = std::mt19937(std::random_device{}());
};

class MemeFighter
{
public:
	MemeFighter(std::string _name, int _hp, int _speed, int _power)
		: name(_name), hp(_hp), speed(_speed), power(_power)
	{}
	virtual void SpecialMove(MemeFighter& otherFighter) = 0;
	virtual void SpecialMove() = 0;

public:
	void Punch(MemeFighter& otherFighter)
	{
		if (IsAlive())
		{
			Dice dice;
			int totalPower = power + dice.Roll(2);
			otherFighter.hp -= totalPower;
			std::cout << name << " deals " << totalPower << " dmg to " << otherFighter.GetName() << "!" << std::endl;
		}

		if (!otherFighter.IsAlive())
		{
			std::cout << otherFighter.GetName() << " is defeated!" << std::endl;
			std::cout << "---------------------------------------------------------------------\n";
		}
	}
	void Tick()
	{
		if (IsAlive())
		{
			Dice dice;
			int hpRecovered = dice.Roll(1);
			hp += hpRecovered;
			std::cout << name << " recovers " << hpRecovered << " hp, current hp is now " << hp << "!" << std::endl;
		}
	}

	std::string GetName() const {
		return name;
	}
	int GetInitiative() const {
		Dice dice;
		return speed + dice.Roll(2);
	}
	bool IsAlive() const
	{
		return hp > 0;
	};
	void SetHP(int damage)
	{
		hp -= damage;
	}
protected:
	int hp;
	int speed;
	int power;

	void SetName(std::string newName)
	{
		name = newName;
	}

private:
	std::string name;
};

class MemeFrog : public MemeFighter
{
public:
	MemeFrog(std::string _name)
		: MemeFighter(_name, 70, 7, 14)
	{};
public:
	virtual void SpecialMove(MemeFighter& otherFighter) override
	{
		if (IsAlive())
		{
			Dice dice;
			int rollResult = dice.Roll(1);
			if (rollResult >= 5)
			{
				int totalDMG = SpecialMoveDMG + dice.Roll(3);
				otherFighter.SetHP(totalDMG);
				std::cout << GetName() << " is performing special move! " << otherFighter.GetName() << " takes " << totalDMG << "!" << std::endl;
				if (!otherFighter.IsAlive())
				{
					std::cout << otherFighter.GetName() << " died from " << GetName() << "'s special move!" << std::endl;
					std::cout << "---------------------------------------------------------------------\n";
				}

			}
			else
			{
				std::cout << GetName() << "'s special move failed!" << std::endl;
			}
		}

	}
	void Tick()
	{
		MemeFighter::Tick();
		if (IsAlive())
		{
			Dice dice;
			int DMGTaken = dice.Roll(1);
			hp -= DMGTaken;
			std::cout << GetName() << " takes " << DMGTaken << " self DMG!" << std::endl;
			if (!IsAlive())
			{
				std::cout << GetName() << " dies from self DMG!" << std::endl;
				std::cout << "---------------------------------------------------------------------\n";
			}
		}
	}
	virtual void SpecialMove() override
	{}
public:
	int SpecialMoveDMG = 20;
};

class MemeStoner : public MemeFighter
{
public:
	MemeStoner(std::string _name)
		: MemeFighter(_name, 80, 4, 10)
	{}
public:
	void SpecialMove() {
		if (IsAlive())
		{
			Dice dice;
			int rollResult = dice.Roll(1);
			if (rollResult % 2 == 0)
			{
				speed += 3;
				int rollResult2 = dice.Roll(1);
				int powerBoost = 0;
				if (rollResult2 % 2 == 0)
					powerBoost = 69;
				else
					powerBoost = 42;
				power += powerBoost;
				hp += 10;
				std::cout << GetName() << " is performing special move! Gains 3 speed! Gains " << powerBoost << " power! Gains 10 hp! ";
				SetName(GetName().insert(0, "Super "));
				std::cout << "His name is now " << GetName() << "!!" << std::endl;
			}
			else
			{
				std::cout << GetName() << "'s special move failed!" << std::endl;
			}
		}
	}
	void SpecialMove(MemeFighter& otherFighter)
	{}
private:
};

void Engage(MemeFighter& f1, MemeFighter& f2)
{
	// pointers for sorting purposes
	auto* p1 = &f1;
	auto* p2 = &f2;
	// determine attack order
	if (p1->GetInitiative() < p2->GetInitiative())
	{
		std::swap(p1, p2);
	}
	// execute attacks
	p1->Punch(*p2);
	p2->Punch(*p1);
}

int main()
{
	MemeFrog f1("Dat Boi");
	MemeStoner f2("Good Guy Greg");

	while (f1.IsAlive() && f2.IsAlive())
	{
		// trade blows
		Engage(f1, f2);
		// special moves
		f2.SpecialMove();
		f1.SpecialMove(f2);
		// end of turn maintainence
		f1.Tick();
		f2.Tick();

		//std::cout << "Press any key to continue...";
		//int val;
		//std::cin >> val;
		std::cout << std::endl << std::endl;
	}

	if (f1.IsAlive())
	{
		std::cout << f1.GetName() << " is victorious!";
	}
	else if (f2.IsAlive())
	{
		std::cout << f2.GetName() << " is victorious!";
	}
	else
	{
		std::cout << "They are both dead!" << std::endl;
	}
	return 0;
}