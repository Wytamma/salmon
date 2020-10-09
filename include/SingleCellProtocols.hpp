#ifndef __SINGLE_CELL_PROTOCOLS_HPP__
#define __SINGLE_CELL_PROTOCOLS_HPP__

#include <string>
#include <ostream>

#include "AlevinOpts.hpp"
#include "AlevinTypes.hpp"
#include "pufferfish/chobo/static_vector.hpp"

namespace alevin{
  namespace protocols {

    static constexpr size_t num_tag_pieces{16};
    struct TagGeometry {
      uint32_t read_num{0};
      chobo::static_vector<std::pair<uint32_t, uint32_t>, num_tag_pieces> substr_locs{};
      // chobo::static_vector<std::pair<uint32_t,uint8_t>, 16> bc_locs;
      // std::vector<std::pair<uint32_t, uint32_t>> substr_locs{};
      uint32_t length{0};
      uint32_t largest_index{0};

      bool extract(std::string& from, std::string&to) {
        if (from.length() < largest_index) { return false; }
        to.clear();
        for (auto& st_len : substr_locs) {
          to += from.substr(st_len.first, st_len.second);
        }
        return true;
      }
    };

    std::ostream& operator<<(std::ostream& os, const TagGeometry& tg);

    struct Rule{
      Rule(){}
      Rule(uint32_t barcodeLength_,
           uint32_t umiLength_,
           BarcodeEnd end_,
           uint32_t maxValue_):
        barcodeLength(barcodeLength_),
        umiLength(umiLength_),
        end(end_),
        maxValue(maxValue_){
        alevin::types::AlevinUMIKmer::k(umiLength);
      }
      // NOTE: these do nothing but satisfy 
      // template requirements right now
      void set_umi_geo(TagGeometry& g) { (void)g; };
      void set_bc_geo(TagGeometry& g) { (void)g; };
      uint32_t barcodeLength, umiLength, maxValue;
      BarcodeEnd end;
    };


    struct DropSeq : Rule{
      //Drop-Seq starts from 5 end with 12 length
      //barcode and 8 length umi & iupac can be
      //changed
      DropSeq(): Rule(12, 8, BarcodeEnd::FIVE, 16777216){}
    };

    struct InDrop : Rule{
        //InDrop starts from 5end with variable
        //length barcodes so provide the full
        // length of the barcod eincluding w1.
        // UMI length is 6
      InDrop(): Rule(42, 6, BarcodeEnd::FIVE, 22347776){}

      std::string w1;
      void setW1(std::string& w1_){
        w1 = w1_;
      }
    };

    struct CITESeq : Rule{
      CITESeq(): Rule(16, 10, BarcodeEnd::FIVE, 4294967295){
        featureLength = 15;
        featureStart = 10;
      }

      size_t featureLength, featureStart;
      void setFeatureLength(size_t length) { featureLength = length; }
      void setFeatureStart(size_t startIdx) { featureStart = startIdx; }
    };

    struct ChromiumV3 : Rule{
      ChromiumV3(): Rule(16, 12, BarcodeEnd::FIVE, 4294967295){}
    };

    struct Chromium : Rule{
      Chromium(): Rule(16, 10, BarcodeEnd::FIVE, 4294967295){}
    };

    struct Gemcode : Rule{
      Gemcode(): Rule(14, 10, BarcodeEnd::FIVE, 268435456){}
    };

    struct QuartzSeq2 : Rule{
      QuartzSeq2(): Rule(15, 8, BarcodeEnd::FIVE, 1073741824){}
    };

    struct CELSeq : Rule{
      // WEHI SCORE's CEL-Seq2 starts from 5' end with a 8 bp barcode
      // and a 6 bp UMI.
      CELSeq(): Rule(8, 6, BarcodeEnd::FIVE, 65536){}
    };
    struct CELSeq2 : Rule{
      // WEHI SCORE's CEL-Seq2 starts from 5' end with a 8 bp barcode
      // and a 6 bp UMI.
      CELSeq2(): Rule(6, 6, BarcodeEnd::FIVE, 4096){}
    };

    //dummy class
    struct Custom : Rule{
      Custom() : Rule(0,0,BarcodeEnd::FIVE,0){}
    };
    

    // for the new type of specification
    struct CustomGeometry {
      // vector of offset, length pairs
      TagGeometry umi_geo;
      TagGeometry bc_geo;

      void set_umi_geo(TagGeometry& g) { umi_geo = g; umiLength = umi_geo.length; };
      void set_bc_geo(TagGeometry& g) { bc_geo = g; barcodeLength = bc_geo.length; };

      // These values do nothing in this class except
      // maintain template compat ... fix this design later.
      uint32_t barcodeLength, umiLength, maxValue;
      BarcodeEnd end;
    };

  }
}

#endif
