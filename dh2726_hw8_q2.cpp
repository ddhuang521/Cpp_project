#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

const int side = 20; // square world side length

class Organism{
	public:
		Organism():x(0), y(0), off_x(0), off_y(0), age(0), dead(false), birth(false) {}//constructor
		// virtual functions move and breed
		virtual void move(const int &m, char map[]) {}
		virtual void breed(int m, char map[]) {}
		//mutator
		void set_xy(int new_x, int new_y){ x = new_x; y = new_y; }
		void set_off_xy(int new_x, int new_y){ off_x = new_x; off_y = new_y; }
		void set_age(int new_age){ age = new_age; }
		void set_dead(bool new_dead){dead = new_dead;}
		void set_birth(bool new_birth){birth = new_birth;}
		//accessor
		int get_x() const { return x; }
		int get_y() const { return y; }
		int get_age() const { return age; }
		int get_off_x() const { return off_x; } //offspring location x
		int get_off_y() const { return off_y; } //offspring location y
		bool get_dead() const { return dead; }
		bool get_birth() const { return birth; }
		//auxillary function, determine if a point is within the world
		friend bool in_range (const int &new_x, const int &new_y);

	protected: //protected data, can be accessed directly by class Ant and Dbug
		int x; 
		int y;
		int off_x;
		int off_y;
		int age;
		bool dead;
		bool birth;
};

// derived class Ant
class Ant: public Organism{
	public:
		Ant(): Organism() {}//constructor
		void move(const int &m, char map[]);	//Ant moves	
		void breed(int m, char map[]);//give birth to a new ant 
};

// derived class Dbug: it can eat and starve
class Dbug: public Organism{
	public:
		Dbug(): Organism(), eating(-1), un_feed_time(0) {} //constructor, no ant is eaten, eating = -1
		void set_eating(int new_eating){ eating = new_eating;}
		int get_eating() const {return eating;} //access the ant's location that this Dbug eats
		void eat(int m, char map[]);	//Dbug eats	
		void move(const int &m, char map[]);	//doodlebugs breeding function
		void breed(int m, char map[]);		//decide if the doodlebug starves
		void starve(char map[]);
				
	private:
		int eating;
		int un_feed_time;
};

class World{ // create a visual world 
	public:
		World(): empty(true), ant_num(0), db_num(0) {}//constructor
		
		/*the world is a square char world stored in char array map and the coordinate(x,y) is built as follows:
		(0,0) (1,0) (2,0) (3,0) ....... (side - 1,0)
		(0,1) .              				.
		(0,2)    .							.
		(0,3)       .						.
		.              .						.
		.                 .					.
		.                   .				.
		.
		(0, side - 1)        ......  (side - 1, side - 1)                     */
	
		World(char map[]); //constructor
		void display_number(){cout<<"Ants #: "<<ant_num<<", Doodlebugs #: "<<db_num<<endl<<endl;}//display the number of ants and doodlebugs living on the world
		void initial_map(Ant a[], Dbug d[], const int &ant_size, const int &db_size, char map[]); // randomly place ants and dbugs on the map with expected numbers
		bool is_empty(const int &x, const int &y, char map[]);//check if a location is empty
		void next(Ant a[], Dbug d[], char map[]);//take the world to the next time step		
		void display(char map[]);//display the world to the screen
		int find_ind(const int &num, Ant a[], const int &ant_size);//used to find the index of a ant in its array which is eaten by the doodledog, make it possible to set that ant as dead later
			
	private:
		bool empty;
		int ant_num;
		int db_num;
};


int main() {
	
	//create the char world map, set the number of ants and doodlebugs in the world, and the world time t = 0
	char map[side * side];	
	int ant_size = 100, db_size = 5;
	int t = 0;
	//create Ant type and Dbug type array with the maximum size which is just the world size;
	//the max size is only be used when one specie dominates and the other extincts
	Ant a[side * side];
	Dbug d[side * side];
	
	//create World type world
	World my_world(map);
	cout<<"Press Enter to the next time step, enter q to exit."<<endl;//instruction for the user
	cout<<endl;
	//initilaize the world map with organism
	my_world.initial_map(a,d,ant_size,db_size,map);
	cout<<"t = "<<t<<" "; //show the current time
	my_world.display_number();//show the current number of ants and doodlebugs
	my_world.display(map);//show the world map

	char ch;//auxilliary variable to let the user makes the world time moves forward
	while(cin.get(ch) && ch != 'q'){ //read user input, enter to move forward, q to quit
		t++;//world time plus 1
		cout<<"t = "<<t<<" "; 
		my_world.next(a, d, map); //change the world
		my_world.display_number(); 
   		my_world.display(map);
	}
	
	return 0;
}


bool in_range(const int &new_x, const int &new_y){
	bool range;
	if(new_x < 0 || new_x > side - 1 || new_y < 0 || new_y > side - 1 ){
		range = false;
	}
	else{
		range = true;
	}
	return range;
}

void Ant::move(const int &m, char map[]){ //virtual functional move 

	bool up_empty = false, down_empty = false, right_empty = false, left_empty = false;
	
	// check if there exists adjacent empty space
	if(in_range(x, y + 1) == true){
		if(map[(y + 1) * side + x] == '-'){
			down_empty = true;
		}
	}	
	if(in_range(x, y - 1) == true){
		if(map[(y - 1) * side + x] == '-'){
			up_empty = true;
		}
	}	
	if(in_range(x + 1, y) == true){
		if(map[y * side + x + 1] == '-'){
			right_empty = true;
		}
	}
	if(in_range(x - 1, y) == true){
		if(map[y * side + x - 1] == '-'){
			left_empty = true;
		}
	}
	
	// if adjacent empty space exist and it is also the direction the ant intents to move then MOVE.			
	if(m == 0 && up_empty == true){
		map[y * side + x] = '-';
		y = y - 1;
		map[y * side + x] = 'o';
	}
	else if(m == 1 && right_empty == true){
		map[y * side + x] = '-';
		x =  x +  1;
		map[y * side + x] = 'o';
	}
	else if(m == 2 && down_empty == true){
		map[y * side + x] = '-';
		y = y + 1;
		map[y * side + x] = 'o';
	}
	else if(m == 3 && left_empty == true){
		map[y * side + x] = '-';
		x = x - 1;
		map[y * side + x] = 'o';
	}	
	
}

void Ant::breed(int m, char map[]){

	bool up_empty = false, down_empty = false, right_empty = false, left_empty = false;
	// check if adjacent empty space available for breeding
	if(in_range(x, y - 1) == true){
		if(map[(y - 1) * side + x] == '-' ){
			up_empty = true;
		}
	}	
	if(in_range(x, y + 1) == true){
		if(map[(y + 1) * side + x] == '-'){
			down_empty = true;
		}
	}	
	if(in_range(x + 1, y) == true){
		if(map[y * side + x + 1] == '-'){
			right_empty = true;
		}
	}
	if(in_range(x - 1, y) == true){
		if(map[y * side + x - 1] == '-'){
			left_empty = true;
		}
	}
	
	// if there exists adjacent empty space;
	if(up_empty == true || down_empty == true || left_empty == true || right_empty == true){
		
		// if the random direction is not empty, create a new direction until it is empty for breeading
		while(birth == false){
			m = rand() % 4;
			if(m == 0 && up_empty == true){
				map[(y - 1) * side + x] = 'o';
				off_x = x;
				off_y = y - 1;
				birth = true;
			}
			else if(m == 1 && right_empty == true){
				map[y * side + x + 1] = 'o';
				off_x = x + 1;
				off_y = y;
				birth = true;
			}
			else if(m == 2 && down_empty == true){
				map[(y + 1) * side + x] = 'o';
				off_x = x;
				off_y = y + 1;
				birth = true;
			}
			else if(m == 3 && left_empty == true){
				map[y * side + x - 1] = 'o';
				off_x = x - 1;
				off_y = y;
				birth = true;
			}
			else{
				birth = false;
			}
		} //while
		
	}//if
	else{
		birth = false;
	}
		
}

void Dbug::eat(int m, char map[]){ 
	eating = -1;// update to not eat yet
	bool up_ant = false, down_ant = false, right_ant = false, left_ant = false;
	//check if there exits adjacent ant available for eating
	if(in_range(x, y - 1) == true){
		if(map[(y - 1) * side + x] == 'o'){
			up_ant = true;
		}
	}	
	if(in_range(x, y + 1) == true){
		if(map[(y + 1) * side + x] == 'o'){
			down_ant = true;
		}
	}	
	if(in_range(x + 1, y) == true){
		if(map[y * side + x + 1] == 'o'){
			right_ant = true;
		}
	}
	if(in_range(x - 1, y) == true){
		if(map[y * side + x - 1] == 'o'){
			left_ant = true;
		}
	}
	
	//if there exists adjacent ants for eating, randomly eat one
	if(up_ant == true || down_ant == true || left_ant == true || right_ant == true){
		while(eating == -1){
			m = rand() % 4; // re-random a direction until it has available ant for eat
			if(m == 0 && up_ant == true){
				map[y * side + x] = '-';
				y = y - 1;
				map[y * side + x] = 'X';
				eating = y * side + x; //update the location where the ant is eaten
			}
			else if(m == 1 && right_ant == true){
				map[y * side + x] = '-';
				x =  x + 1;
				map[y * side + x] = 'X';	
				eating = y * side + x;
			}
			else if(m == 2 && down_ant == true){
				map[y * side + x] = '-';
				y = y + 1;
				map[y * side + x] = 'X';
				eating = y * side + x;
			}
			else if(m == 3 && left_ant == true){
				map[y * side + x] = '-';
				x = x - 1;
				map[y * side + x] = 'X';
				eating = y * side + x;
			}
			else{
				eating = -1;
			}
		}
		un_feed_time = 0; // after eating reset the unfeed time to 0
	}
	
}

void Dbug::move(const int &m, char map[]){
	un_feed_time++; //increment the unfeed time by 1
	bool up_empty = false, down_empty = false, right_empty = false, left_empty = false;
	
	//check if there exists adjacent empty space for movement
	if(in_range(x, y - 1) == true){
		if(map[(y - 1) * side + x] == '-'){
			up_empty = true;
		}
	}	
	if(in_range(x, y + 1) == true){
		if(map[(y + 1) * side + x] == '-'){
			down_empty = true;
		}		
	}	
	if(in_range(x + 1, y) == true){
		if(map[y * side + x + 1] == '-'){
			right_empty = true;
		}
	}
	if(in_range(x - 1, y) == true){
		if(map[y * side + x - 1] == '-'){
			left_empty = true;
		}
	}
	
	//if there exists empty space for move and it is also the intended direction then MOVE
    if(up_empty == true || down_empty == true || left_empty == true || right_empty == true){
	
		  if(m == 0 && up_empty == true){
			  map[y * side + x] = '-';
			  y = y - 1;
			  map[y * side + x] = 'X';
		  }
		  else if(m == 1 && right_empty == true){
			  map[y * side + x] = '-';
		 	  x =  x + 1;
			  map[y * side + x] = 'X';
		  }
		  else if(m == 2 && down_empty == true){
			  map[y * side + x] = '-';
			  y = y + 1;
			  map[y * side + x] = 'X';
		  }
		  else if(m == 3 && left_empty == true){
			  map[y * side + x] = '-';
			  x = x - 1;
			  map[y * side + x] = 'X';
		  }
     }

}

void Dbug::breed(int m, char map[]){
	//check if there are available space for breeding
	bool up_empty = false, down_empty = false, right_empty = false, left_empty = false;
	
	if(in_range(x, y - 1) == true){
		if(map[(y - 1) * side + x] == '-'){
			up_empty = true;
		}
	}	
	if(in_range(x, y + 1) == true){
		if(map[(y + 1) * side + x] == '-'){
			down_empty = true;
		}
	}	
	if(in_range(x + 1, y) == true){
		if(map[y * side + x + 1] == '-'){
			right_empty = true;
		}
	}
	if(in_range(x - 1, y) == true){
		if(map[y * side + x - 1] == '-'){
			left_empty = true;
		}
	}
	//if there are available space
	if(up_empty == true || down_empty == true || left_empty == true || right_empty == true){
		//randomly breed the offspring to the empty space;
		while(birth == false){
			m = rand() % 4;
			if(m == 0 && up_empty == true){
				map[(y - 1) * side + x] = 'X';
				off_x = x;
				off_y = y - 1;
				birth = true;
			}
			else if(m == 1 && right_empty == true){
				map[y * side + x + 1] = 'X';
				off_x = x + 1;
				off_y = y;
				birth = true;
			}
			else if(m == 2 && down_empty == true){
				map[(y + 1) * side + x] = 'X';
				off_x = x;
				off_y = y + 1;
				birth = true;
			}
			else if(m == 3 && left_empty == true){
				map[y * side + x - 1] = 'X';
				off_x = x - 1;
				off_y = y;
				birth = true;
			}
			else{
				birth = false;
			}
		} 
		
	}
	
	else{
		birth = false;
	}

}

void Dbug::starve(char map[]){
	if(un_feed_time == 3){ //if unfeed after 3 time steps
		map[y * side + x] = '-'; //efface the doodlebug on the map
		dead = true;//set dead to true
	}
}

World::World(char map[]){ //initial the world to empty world
	for(int i = 0; i < side; i++){
		for(int j = 0; j < side; j++){
			map[i * side + j] = '-';
		}
	}
	empty = true;
	ant_num = 0;
	db_num = 0;
} //constructor

void World::initial_map(Ant a[], Dbug d[], const int &ant_size, const int &db_size, char map[]){ // randomly place ants and dbugs on the map with expected numbers
	int x, y;
	int counter1 = 0, counter2 = 0;
	//store the user input ant and doodledog numbers to the World class variables for furture reference
	db_num = db_size;
	ant_num = ant_size;
	
	srand(time(0));
	while(counter1 < ant_size){ //randomly generate ants
		x = (rand() % (side * side)) % side;
		y = (rand() % (side * side)) % side;
		if (is_empty(x,y,map) == true){ //check if the planned location is empty
			map[y * side + x] = 'o';
			a[counter1].set_xy(x,y); 
			counter1++;
		}
	}
	
	while(counter2 < db_size){ //randomly generate doodlebugs
		x = (rand() % (side * side)) % side;
		y = (rand() % (side * side)) % side;
		if (is_empty(x,y,map) == true){
			map[y * side + x] = 'X';
			d[counter2].set_xy(x,y);
			counter2++;
		}
	}
}

bool World::is_empty(const int &x, const int &y, char map[]){
	if(map[y * side + x] == '-'){
		empty = true;
	}
	else{
		empty = false;
	}
	return empty;
}

void World::next(Ant a[], Dbug d[], char map[]){ //update the ants, doodlebugs and world map information for each time step
	//initialize auxilliary Ant and Dbug array
	Ant a_temp[side * side];
	Dbug d_temp[side * side];
	//initialize temporary ant and doodlebug size
	int temp_size_ant = ant_num;
	int temp_size_db = db_num;
	int db_max = db_num;
	
	srand(time(0));
	int m, ind; //random direction indicator m; index of the ant which is eaten
	
	for(int i = 0; i < temp_size_db; i++){
	    ind = 0;
		m = rand() % 4; //randomize a direction
		d[i].set_age(d[i].get_age() + 1); //aging the doodlebug
		d[i].eat(m, map); //let the doodlebug check if it can eat any ant
		
		if(d[i].get_eating() != -1){ //if doodlebug eats a ant 
			ind = find_ind(d[i].get_eating(), a, temp_size_ant); //find the index of that ant in the Ant array a
			a[ind].set_dead(true); //record that ant is dead
			ant_num--; //ant number decreases by 1
		} 
		else{//if no ant available for eating
			d[i].move(m,map); //then just move to an adjacent empty space
		}
		
		//if the age of this doodlebug is 8 or 8's multiples, then it should breed				
		if(d[i].get_age() % 8 == 0){
			d[i].set_birth(false);
			d[i].breed(m, map); // breed offspring on a random available space
			if(d[i].get_birth() == true){
				d[db_max].set_xy(d[i].get_off_x(), d[i].get_off_y()); //add the offspring to Dbug array d, set its birth location
				db_num++; //doodlebug number plus 1
				db_max++; //Dbug array used element plus 1
			}
		}
		
		d[i].starve(map); //check if the doodlebug starves, if starving, efface it from the world map
		if(d[i].get_dead() == true){ //if starves
			db_num--; // doodlebug number minus 1
		}
	}
	
	
	/* Extract the dead organism from Ant and Dbug array, keep the ants and doodlebugs in their array alive */
	int counter_ant = 0;
	int counter_db = 0;
	for(int i = 0; i < temp_size_ant; i++){//temp_size_ant is max possible ants alive
		if(a[i].get_dead() == false){ // if the ant is alive
			a_temp[counter_ant] = a[i]; // add the ant to the temporary array
			counter_ant++;
		}
	}
	
	for(int i = 0; i < db_max; i++){//db_max is the max possible doodlebugs alive (cannot use temp_size_db bc it has been used as the doodlebug forloop parameter, cannot be changed there, while db_max can be changed)
		if(d[i].get_dead() == false){ //add the doodlebug alive to the temp array
			d_temp[counter_db] = d[i];
			counter_db++;
		}
	}	
	temp_size_ant = ant_num;//update the temp ant size to current ant number after some of the ants are eaten
			
	for(int i = 0; i < temp_size_ant; i++){ // let the ants move
		m = rand() % 4; // random direction index
		
		a_temp[i].move(m, map); //ant moves
		a_temp[i].set_age(a_temp[i].get_age() + 1); //ant ages
		
		
		if(a_temp[i].get_age() % 3 == 0){ //if the ant's age is 3 or 3's multiple, it breeds
			a_temp[i].set_birth(false);
			a_temp[i].breed(m, map); //check availbe location and breed

			if(a_temp[i].get_birth() == true){ //if breed
				a_temp[ant_num].set_xy(a_temp[i].get_off_x(), a_temp[i].get_off_y()); //add the offspring to the Ant array
				ant_num++; //increase ant number
			}
		}

	}
	
	//use the temporary array to update the original array, to make sure all the organism in original ararry are alive at the beginning of each time step and erase the vestige in original array
	for(int i = 0; i < side * side; i++){
		a[i] = a_temp[i];
	}
	for(int i = 0; i < side * side; i++){
		d[i] = d_temp[i];
	}
	
}

void World::display(char map[]){
	for(int i = 0; i < side; i++){
		for(int j = 0; j < side; j++){
			cout<<map[i*side + j]<<"  ";
		}
		cout<<endl;
	}
}

int World::find_ind(const int &num, Ant a[], const int &ant_size){
	int i = 0;
	int pos;
	while(i < ant_size){
		pos = a[i].get_y() * side + a[i].get_x();
		if( pos == num){
			return i;
		}
		i++;
	}
	
	return -1; // won't be executed unless error exists in the ant array
}
