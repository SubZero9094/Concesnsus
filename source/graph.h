#include <iostream>
#include <string>
#include <vector> 
#include <utility>


#include "../../Empirical/source/hardware/EventDrivenGP.h"
#include "../../Empirical/source/base/vector.h"
#include "../../Empirical/source/tools/Random.h"
#include "../../Empirical/source/tools/random_utils.h"

class Graph
{
	public:
		static constexpr size_t TAG_WIDTH = 16;

		using hardware_t = emp::EventDrivenGP_AW<TAG_WIDTH>;

		Graph(std::string name = "Toroidal", size_t dim = 5, int seed=-1) : mName(name), mDimension(dim)
		{
			//Set up matrix
			SetupGraph(mDimension);

			mRng = emp::Random(seed);

			if (mName == "Toroidal")
			{
				SetupToroidal(mDimension);
			}
		}

		//Will great an nxn graph to hold the an individual hardware per node
		//in the matrix.
		void SetupGraph(size_t n)
		{
			mMat.resize(n);
			for(auto &i : mMat)
			{
				i.resize(n);
			}
		}

		//Give each node its neighboors list. Assuming the matrix top left corner is the 
		//coordinate (0,0), right -> (0,1), left -> (n,0), up -> (0,n), down -> (0,-1).
		//The reason we add n to the coordinates is to deal with the negative if at an edge.
		void SetupToroidal(size_t n)
		{
			for(size_t i = 0; i < mMat.size(); ++i)
			{
				for(size_t j = 0; j < mMat[i].size(); ++j)
				{
					//Right
					mMat[i][j].friends.emplace_back(i,(j+1+n)%n);
					//Left
					mMat[i][j].friends.emplace_back(i,(j-1+n)%n);
					//Up
					mMat[i][j].friends.emplace_back((i-1+n)%n,j);
					//Down
					mMat[i][j].friends.emplace_back((i+1+n)%n,j);
					//Add Coordinate to the scheduler
					mSchedule.emplace_back(i,j);

				}
			}
		}

		//Print the neighboors list from each node
		void PrintFriends()
		{
			std::cout << "Coor --> RIGHT, LEFT, UP, DOWN" << std::endl;
			for(size_t i = 0; i < mMat.size(); ++i)
			{
				for(size_t j = 0; j < mMat[i].size(); ++j)
				{
					std::cout << '(' << i << "," << j << ")--> ";
					for(auto p : mMat[i][j].friends)
					{
						std::cout << "(" << p.first << "," << p.second << "), ";
					}
					std::cout << std::endl;
				}
			}
		}

		//Print the schedule
		void PrintSchedule()
		{
			for(auto p : mSchedule)
			{
				std::cout << "(" << p.first << "," << p.second << ")" << std::endl;
			}
		}

		//Reshuffle the schedule 
		void ShuffleSchedule(){	emp::Shuffle(mRng, mSchedule);}

		//A node will hold its neighbors and hold its hardware
		struct Node
		{
			hardware_t cell;
			std::vector<std::pair<size_t, size_t>> friends;
		};		

		//Return the kind of graph it is.
		std::string GetName() { return mName; }
		//Return the Dimensionality.
		size_t GetDim() { return mDimension; }

	private:
		//Name of the graph
		std::string mName;

		//Dimensionality of the structure nxn
		size_t mDimension;

		//Actual matrix structure
		std::vector<std::vector<Node>> mMat;

		//List of who goes when throughout the experiment
		emp::vector<std::pair<size_t,size_t>> mSchedule;

		//Random Controller
		emp::Random mRng;
};