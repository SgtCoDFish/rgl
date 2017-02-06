#ifndef INCLUDE_STATS_HPP_
#define INCLUDE_STATS_HPP_

namespace rgl {

struct Stats {
	int maxHP;
	int hp;

	int atk;
	int def;

	int speed;

	explicit Stats(int maxHP, int atk, int def, int speed = 0) :
			maxHP { maxHP }, hp { maxHP }, atk { atk }, def { def }, speed { speed } {
	}

	Stats operator +(const Stats &other) const {
		return Stats(maxHP + other.maxHP, atk + other.atk, def + other.def, speed + other.speed);
	}

	Stats operator -(const Stats &other) const {
		return Stats(maxHP - other.maxHP, atk - other.atk, def - other.def, speed - other.speed);
	}

	Stats &operator+=(const Stats &other) {
		maxHP += other.maxHP;
		atk += other.atk;
		def += other.def;
		speed += other.speed;

		return *this;
	}

	Stats &operator-=(const Stats &other) {
		maxHP -= other.maxHP;
		atk -= other.atk;
		def -= other.def;
		speed -= other.speed;

		return *this;
	}
};

}

#endif /* INCLUDE_STATS_HPP_ */
