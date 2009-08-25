
#include "lib/common.h"
#include "features/DotFeatures.h"
#include "features/SimpleFeatures.h"



class CPolyFeatures : public CDotFeatures
{
	public:
		/** constructor
		 * 
		 * @param feat real features
		 * @param degree degree of the polynomial kernel
		 */
		CPolyFeatures(CSimpleFeatures<float64_t>* feat, int32_t degree, bool normalize);

		virtual ~CPolyFeatures();

		CPolyFeatures(const CPolyFeatures & orig){ 
			SG_PRINT("CPolyFeatures:\n");
			SG_NOTIMPLEMENTED;};

		inline virtual int32_t get_dim_feature_space()
		{
			return m_output_dimensions;
		}

		virtual inline int32_t get_nnz_features_for_vector(int32_t num)
		{
			return m_output_dimensions;
		}
		inline virtual EFeatureType get_feature_type()
		{
			return F_UNKNOWN;
		}
		inline virtual EFeatureClass get_feature_class()
		{
			return C_POLY;
		}

		inline virtual int32_t get_num_vectors()
		{
			if (m_feat)
				return m_feat->get_num_vectors();
			else
				return 0;

		}

		virtual float64_t dot(int32_t vec_idx1, int32_t vec_idx2);

		inline virtual int32_t get_size()
		{
			return sizeof(float64_t);
		}

		CFeatures* duplicate() const;

		inline virtual const char* get_name() const { return "PolyFeatures"; }
		/** compute dot product of vector with index arg1 
		 *  with an given second vector */
		float64_t dense_dot(int32_t vec_idx1, const float64_t* vec2, int32_t vec2_len);

		/** compute alpha*x+vec2*/
		void add_to_dense_vec(float64_t alpha, int32_t vec_idx1, float64_t* vec2, int32_t vec2_len, bool abs_val);

	protected: 

		/** store the norm of each training example */
		void store_normalization_values();

		/** caller function for the recursive function enumerate_multi_index */
		void store_multi_index();

		/** recursive function enumerating all multi-indices that sum 
		 *  up to the degree of the polynomial kernel */
		void enumerate_multi_index(const int32_t feat_idx, uint16_t** index, uint16_t* exponents, const int32_t degree);
		/** function calculating the multinomial coefficients for all 
		 *  multi indices */
		void store_multinomial_coefficients();

		/** simple recursive implementation of binomial coefficient 
		 *  which is very efficient if k is small, otherwise it calls 
		 *  a more sophisticated implementation */
		int32_t bico2(int32_t n, int32_t k);

		/** efficient implementation for the binomial coefficient function 
		 *  for larger values of k*/
		int32_t  bico(int32_t n, int32_t k);

		/** recursion to calculate the dimensions of the feature space:
		 *  A(N, D)= sum_d=0^D A(N-1, d) 
		 *  A(1, D)==1
		 *  A(N, 0)==1
		 *  where N is the dimensionality of the input space 
		 *  and D is the degree */
		int32_t calc_feature_space_dimensions(int32_t N, int32_t D);

		/** calculate the multinomial coefficient */
		int32_t multinomialcoef(int32_t* exps, int32_t len);

		/** efficient implementation of the ln(gamma(x)) function*/
		float64_t gammln(float64_t xx);

		/** implementation of the ln(x!) function*/
		float64_t factln(int32_t n);

	protected:

		/** features in original space*/
		CSimpleFeatures<float64_t>* m_feat;
		/** degree of the polynomial kernel */
		int32_t m_degree;
		/** normalize */
		bool m_normalize;
		/** dimensions of the input space */
		int32_t m_input_dimensions;
		/** dimensions of the feature space of the polynomial kernel */
		int32_t m_output_dimensions;
		/** flattened matrix of all multi indices that 
		 *  sum do the degree of the polynomial kernel */
		uint16_t* m_multi_index;
		/** multinomial coefficients for all multi-indices */
		float64_t* m_multinomial_coefficients; 
		/**store norm of each training example */
		float32_t* m_normalization_values; 

};
