//
// Created by 24565 on 6/1/2022.
//

#include "RecommenderSystemLoader.h"
#include "RSUsersLoader.h"



/** todo delete  */

#include <iostream>
#include "Movie.h"
#include "RecommenderSystem.h"
using namespace std; //todo not good, use seperate stds for relevants

/** todo delete  */


int main(){
  Movie m("A",1999);
  cout << m << endl;
  unique_ptr<RecommenderSystem> rs = RecommenderSystemLoader::create_rs_from_movies_file ("../RecommenderSystemLoader_input.txt");
  cout<<*rs<<endl;
  vector<RSUser> users = RSUsersLoader::create_users_from_file("../RSUsersLoader_input.txt",move(rs));
  for(const auto& user: users)
  {
    cout<<user<<endl;
    for(const auto& f:user.get_ranks())
    {
      cout<<f.first->get_name()<<","<<f.second<<" "<<endl;
    }
  }
//  sp_movie check1 = users[0].get_recommendation_by_content();
//  cout<<check1->get_name()<<endl;
//  sp_movie check = users[0]._rec_sys->get_movie ("lior",1000);
//  cout<<(check.get() == nullptr)<<endl;
//  auto x = users[1].get_prediction_score_for_movie("Twilight",2008,2);
//  cout<<x<<endl;
//  sp_movie mov = users[0]._rec_sys->get_movie("Twilight", 2008);
//  double predict = users[0]._rec_sys->predict_movie_score(users[2], mov, 2);
//  cout<<(users[2].get_recommendation_by_cf (2)->get_name())<<endl;
//  cout << predict << endl;

  cout<<"*******************************************************\n";
  RSUser omri = users[0];
  assert( omri.get_name() == "Omri");
  cout<<"precidtion for TITANIC 1997 by 2 movies for omri: " << omri
  .get_prediction_score_for_movie ("Titanic",1997,2) <<endl;
  auto rec_by_ct = omri.get_recommendation_by_content();
  cout<<rec_by_ct->get_name() << " " <<rec_by_ct->get_year() <<endl;








//  try
//  {
//    auto rs = RecommenderSystemLoader::create_rs_from_movies_file
//        ("../RecommenderSystemLoader_input.txt");
////    printer(*rs);
//    auto rsu = RSUsersLoader::create_users_from_file
//        ("../RSUsersLoader_input.txt",std::move(rs));
//  }
//  catch(runtime_error& run)
//  {
//    cout << run.what();
//    return EXIT_FAILURE;
//  }
//  catch(length_error& len)
//  {
//    cout << len.what();
//    return EXIT_FAILURE;
//  }
//
//

  return EXIT_SUCCESS;
}