#ifndef TEXTNET_NET_TRAIN_VALID_TEST_NET_INL_HPP_
#define TEXTNET_NET_TRAIN_VALID_TEST_NET_INL_HPP_

#include <mshadow/tensor.h>
#include "./net.h"

namespace textnet {
namespace net {

template<typename xpu>
class TrainValidTestNet : public Net<xpu>{
 public:
  TrainValidTestNet() { this->net_type = kTrainValidTest; }
  virtual ~TrainValidTestNet(void) {}
  
  virtual void Start() {

	this->SetupAllNets();

	utils::Check(this->nets.count("Train"), 
			"No [Train] tag in config.");
	utils::Check(this->nets.count("Valid"),
			"No [Valid] tag in config.");
	utils::Check(this->nets.count("Test"),
			"No [Test] tag in config.");
    

	for (int iter = 0; iter < this->max_iters["Train"]; ++iter) {
		this->TrainOneStep("Train");

		if (this->display_interval["Train"] > 0 && iter % this->display_interval["Train"] == 0) {
			this->TrainDisplay("Train", iter);
		}

		if (this->display_interval["Valid"] > 0 && iter % this->display_interval["Valid"] == 0) {
			this->TestAll("Valid", iter);
		}	

		if (this->display_interval["Test"] > 0 && iter % this->display_interval["Test"] == 0) {
			this->TestAll("Test", iter);
		}	

		if (this->save_interval["Train"] > 0 && iter % this->save_interval["Train"] == 0) {
            this->SaveModel("Train", iter);
		}
	}
  } 
};
}  // namespace net
}  // namespace textnet
#endif  // NET_TRAIN_VALID_TEST_NET_INL_HPP_

