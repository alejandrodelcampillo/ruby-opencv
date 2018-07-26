#include "opencv2/dnn.hpp"

#include "opencv.hpp"
#include "mat.hpp"
#include "size.hpp"
#include "scalar.hpp"
#include "dnn_net.hpp"
#include "dnn_layer.hpp"
#include "error.hpp"

// https://docs.opencv.org/trunk/d6/d0f/group__dnn.html#ga29d0ea5e52b1d1a6c2681e3f7d68473a
// https://github.com/opencv/opencv/blob/master/modules/dnn/src/caffe/caffe_importer.cpp

namespace rubyopencv {
  namespace Dnn {
    VALUE rb_module = Qnil;

    // Mat blobFromImage(const Mat& image, double scalefactor=1.0, const Size& size = Size(), const Scalar& mean = Scalar(), bool swapRB=true)
    VALUE rb_blob_from_image(int argc, VALUE *argv, VALUE self) {
      VALUE image, options;
      rb_scan_args(argc, argv, "11", &image, &options);

      cv::Mat *b = NULL;
      cv::Mat *m = Mat::obj2mat(image);

      try {
        cv::Mat r;

        if (NIL_P(options)) {
          r = cv::dnn::blobFromImage(*m);
        } else {
          Check_Type(options, T_HASH);

          double scale_factor = NUM2DBL_DEFAULT(HASH_LOOKUP(options, "scale_factor"), 1.0);
          cv::Size size;
          cv::Scalar mean;
          bool swap_rb = RTEST_DEFAULT(HASH_LOOKUP(options, "swap_rb"), true);
          bool crop = RTEST_DEFAULT(HASH_LOOKUP(options, "crop"), true);
      	  VALUE tmp = Qnil;

      	  tmp = HASH_LOOKUP(options, "size");
      	  if (!NIL_P(tmp)) {
      	    size = *(Size::obj2size(tmp));
      	  }

      	  tmp = HASH_LOOKUP(options, "mean");
      	  if (!NIL_P(tmp)) {
      	    mean = *(Scalar::obj2scalar(tmp));
      	  }

          r = cv::dnn::blobFromImage(*m, scale_factor, size, mean, swap_rb, crop);
        }

        b = new cv::Mat(r);
      } catch(cv::Exception& e) {
        delete b;
        Error::raise(e);
      }

      return Mat::mat2obj(b);
    }

    // // Net readNetFromCaffe(const String &prototxt, const String &caffeModel = String());
    // VALUE rb_read_net_from_caffe(VALUE self, VALUE prototxt, VALUE caffe_model) {
    //   cv::dnn::Net *net = NULL;
    //
    //   try {
    //     net = new cv::dnn::Net(cv::dnn::readNetFromCaffe(StringValueCStr(prototxt), StringValueCStr(caffe_model)));
    //   } catch(cv::Exception& e) {
    //     delete net;
    //     Error::raise(e);
    //   }
    //
    //   return Dnn::Net::net2obj(net);
    // }
    //
    // // Net readNetFromTorch(const String &model, bool isBinary)
    // VALUE rb_read_net_from_tensorflow(VALUE self, VALUE model) {
    //   cv::dnn::Net *net = NULL;
    //
    //   try {
    //     net = new cv::dnn::Net(cv::dnn::readNetFromTensorflow(StringValueCStr(model)));
    //   } catch(cv::Exception& e) {
    //     delete net;
    //     Error::raise(e);
    //   }
    //
    //   return Dnn::Net::net2obj(net);
    // }
    //
    // // Net readNetFromTorch(const String &model, bool isBinary)
    // VALUE rb_read_net_from_torch(VALUE self, VALUE model) {
    //   cv::dnn::Net *net = NULL;
    //
    //   try {
    //     net = new cv::dnn::Net(cv::dnn::readNetFromTorch(StringValueCStr(model)));
    //   } catch(cv::Exception& e) {
    //     delete net;
    //     Error::raise(e);
    //   }
    //
    //   return Dnn::Net::net2obj(net);
    // }
    //
    // // Net readNetFromDarknet(const String &cfgFile, const String &darknetModel /*= String()*/)
    // VALUE rb_read_net_from_darknet(VALUE self, VALUE cfg_file, VALUE darknet_model) {
    //   cv::dnn::Net *net = NULL;
    //
    //   try {
    //     net = new cv::dnn::Net(cv::dnn::readNetFromDarknet(StringValueCStr(cfg_file), StringValueCStr(darknet_model)));
    //   } catch(cv::Exception& e) {
    //     delete net;
    //     Error::raise(e);
    //   }
    //
    //   return Dnn::Net::net2obj(net);
    // }

    void init() {
      VALUE opencv = rb_define_module("Cv");

      rb_module = rb_define_module_under(opencv, "Dnn");

      rb_define_singleton_method(rb_module, "blob_from_image", RUBY_METHOD_FUNC(rb_blob_from_image), -1);
      // rb_define_singleton_method(rb_module, "read_net_from_caffe", RUBY_METHOD_FUNC(rb_read_net_from_caffe), 2);
      // rb_define_singleton_method(rb_module, "read_net_from_tensorflow", RUBY_METHOD_FUNC(rb_read_net_from_tensorflow), 1);
      // rb_define_singleton_method(rb_module, "read_net_from_torch", RUBY_METHOD_FUNC(rb_read_net_from_torch), 1);
      // rb_define_singleton_method(rb_module, "read_net_from_darknet", RUBY_METHOD_FUNC(rb_read_net_from_darknet), 2);

      Dnn::Net::init(rb_module);
      Dnn::Layer::init(rb_module);
    }
  }
}
