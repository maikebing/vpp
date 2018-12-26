#ifndef INC_VPPEXCOMMONMATH_HPP
#define INC_VPPEXCOMMONMATH_HPP

// -----------------------------------------------------------------------------
namespace vppex {
// -----------------------------------------------------------------------------

class Vector4
{
public:
    Vector4();
    Vector4 ( float x, float y, float z );
    Vector4 ( float x, float y, float z, float w );

    float operator[] ( size_t idx ) const;
    float& operator[] ( size_t idx );

    Vector4 normalize() const;
    
    float dot3 ( const Vector4& rhs ) const;
    float dot ( const Vector4& rhs ) const;
    Vector4 cross ( const Vector4& rhs ) const;

    Vector4 operator+ ( const Vector4& rhs ) const;
    Vector4 operator- ( const Vector4& rhs ) const;
    Vector4 operator* ( const Vector4& rhs ) const;

private:
    float m_data [ 4 ];
};

// -----------------------------------------------------------------------------

inline Vector4 :: Vector4()
{
}

// -----------------------------------------------------------------------------

inline Vector4 :: Vector4 ( float x, float y, float z ) :
    m_data { x, y, z, 1.0f }
{
}

// -----------------------------------------------------------------------------

inline Vector4 :: Vector4 ( float x, float y, float z, float w ) :
    m_data { x, y, z, w }
{
}

// -----------------------------------------------------------------------------

inline float Vector4 :: operator[] ( size_t idx ) const
{
    return m_data [ idx ];
}

// -----------------------------------------------------------------------------

inline float& Vector4 :: operator[] ( size_t idx )
{
    return m_data [ idx ];
}

// -----------------------------------------------------------------------------

inline Vector4 Vector4 :: normalize() const
{
    const float length = std::sqrt (
        m_data[0] * m_data[0]
        + m_data[1] * m_data[1]
        + m_data[2] * m_data[2]
    );

    const float invLength = 1.0f / length;

    return Vector4 (
        m_data[0] * invLength,
        m_data[1] * invLength,
        m_data[2] * invLength,
        1.0f
    );
}

// -----------------------------------------------------------------------------

inline float Vector4 :: dot3 ( const Vector4& rhs ) const
{
    return 
        m_data[0] * rhs[0]
        + m_data[1] * rhs[1]
        + m_data[2] * rhs[2];
}

// -----------------------------------------------------------------------------

inline float Vector4 :: dot ( const Vector4& rhs ) const
{
    return 
        m_data[0] * rhs[0]
        + m_data[1] * rhs[1]
        + m_data[2] * rhs[2]
        + m_data[3] * rhs[3];
}

// -----------------------------------------------------------------------------

inline Vector4 Vector4 :: cross ( const Vector4& rhs ) const
{
    return Vector4 (
        m_data[1] * rhs[2] - m_data[2] * rhs[1],
        m_data[2] * rhs[0] - m_data[0] * rhs[2],
        m_data[0] * rhs[1] - m_data[1] * rhs[0],
        1.0f
    );
}

// -----------------------------------------------------------------------------

inline Vector4 Vector4 :: operator+ ( const Vector4& rhs ) const
{
    return Vector4 (
        m_data[0] + rhs[0],
        m_data[1] + rhs[1],
        m_data[2] + rhs[2],
        m_data[3] + rhs[3]
    );
}

// -----------------------------------------------------------------------------

inline Vector4 Vector4 :: operator- ( const Vector4& rhs ) const
{
    return Vector4 (
        m_data[0] - rhs[0],
        m_data[1] - rhs[1],
        m_data[2] - rhs[2],
        m_data[3] - rhs[3]
    );
}

// -----------------------------------------------------------------------------

inline Vector4 Vector4 :: operator* ( const Vector4& rhs ) const
{
    return Vector4 (
        m_data[0] * rhs[0],
        m_data[1] * rhs[1],
        m_data[2] * rhs[2],
        m_data[3] * rhs[3]
    );
}

// -----------------------------------------------------------------------------

inline Vector4 operator* ( float lhs, const Vector4& rhs )
{
    return Vector4 (
        lhs * rhs[0],
        lhs * rhs[1],
        lhs * rhs[2],
        1.0f
    );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class Matrix4
{
public:
    Matrix4();
    Matrix4 ( const std::initializer_list< float >& values );
    Matrix4 ( const Vector4& c0, const Vector4& c1, const Vector4& c2, const Vector4& c3 );

    float operator()( size_t row, size_t col ) const;
    float& operator()( size_t row, size_t col );

    Vector4 row ( size_t col ) const;
    Vector4 column ( size_t col ) const;

    Matrix4 operator+ ( const Matrix4& rhs ) const;
    Matrix4 operator- ( const Matrix4& rhs ) const;
    Matrix4 operator* ( const Matrix4& rhs ) const;
    Matrix4 operator* ( float rhs ) const;

    void setIdentity();

    void setTranslation ( float x, float y, float z );
    void setTranslation ( const Vector4& t );

    void setPerspective (
        float fovy, float aspect, float zNear, float zFar );

    void setLookAt (
        const Vector4& eye,
        const Vector4& center,
        const Vector4& up );

    void setRotation (
        float angle,
        const Vector4& axis );

    Matrix4 transpose();
    Matrix4 inverse();

private:
    float m_data [ 4 ][ 4 ];
};

// -----------------------------------------------------------------------------

inline Matrix4 :: Matrix4()
{
}

// -----------------------------------------------------------------------------

inline Matrix4 :: Matrix4 ( const std::initializer_list< float >& values )
{
    typedef std::initializer_list< float >::const_iterator value_iterator;
    value_iterator v = values.begin();

    for ( unsigned int i = 0; i != 4; ++i )
        for ( unsigned int j = 0; j != 4; ++j )
            m_data [ j ][ i ] = ( v == values.end() ? 0.0f : *v );
}

// -----------------------------------------------------------------------------

inline Matrix4 :: Matrix4 (
    const Vector4& c0, const Vector4& c1, const Vector4& c2, const Vector4& c3 )
{
    const Vector4* pCols [ 4 ] = { & c0, & c1, & c2, & c3 };

    for ( unsigned int i = 0; i != 4; ++i )
        for ( unsigned int j = 0; j != 4; ++j )
            m_data [ i ][ j ] = ( *pCols [ i ] )[ j ];
}

// -----------------------------------------------------------------------------

inline float Matrix4 :: operator()( size_t row, size_t col ) const
{
    return m_data [ col ][ row ];
}

// -----------------------------------------------------------------------------

inline float& Matrix4 :: operator()( size_t row, size_t col )
{
    return m_data [ col ][ row ];
}

// -----------------------------------------------------------------------------

inline Vector4 Matrix4 :: row ( size_t r ) const
{
    return Vector4 (
        m_data [ 0 ][ r ],
        m_data [ 1 ][ r ],
        m_data [ 2 ][ r ],
        m_data [ 3 ][ r ] );
}

// -----------------------------------------------------------------------------

inline Vector4 Matrix4 :: column ( size_t c ) const
{
    return Vector4 (
        m_data [ c ][ 0 ],
        m_data [ c ][ 1 ],
        m_data [ c ][ 2 ],
        m_data [ c ][ 3 ] );
}

// -----------------------------------------------------------------------------

inline Matrix4 Matrix4 :: operator+ ( const Matrix4& rhs ) const
{
    Matrix4 result;

    for ( unsigned int i = 0; i != 4; ++i )
        for ( unsigned int j = 0; j != 4; ++j )
            result.m_data [ i ] [ j ] = m_data [ i ][ j ] + rhs.m_data [ i ][ j ];

    return result;
}

// -----------------------------------------------------------------------------

inline Matrix4 Matrix4 :: operator- ( const Matrix4& rhs ) const
{
    Matrix4 result;

    for ( unsigned int i = 0; i != 4; ++i )
        for ( unsigned int j = 0; j != 4; ++j )
            result.m_data [ i ] [ j ] = m_data [ i ][ j ] - rhs.m_data [ i ][ j ];

    return result;
}

// -----------------------------------------------------------------------------

inline Matrix4 Matrix4 :: operator* ( const Matrix4& rhs ) const
{
    Matrix4 result;

    for ( unsigned int r = 0; r != 4; ++r )
        for ( unsigned int c = 0; c != 4; ++c )
        {
            float v = 0.0f;

            for ( unsigned int k = 0; k != 4; ++k )
                v += m_data [ k ][ r ] * rhs.m_data [ c ][ k ];

            result.m_data [ c ][ r ] = v;
        }

    return result;
}

// -----------------------------------------------------------------------------

inline Matrix4 Matrix4 :: operator* ( float rhs ) const
{
    Matrix4 result;

    for ( unsigned int i = 0; i != 4; ++i )
        for ( unsigned int j = 0; j != 4; ++j )
            result.m_data [ i ][ j ] = m_data [ i ][ j ] * rhs;

    return result;
}

// -----------------------------------------------------------------------------

inline void Matrix4 :: setIdentity()
{
    for ( unsigned int i = 0; i != 4; ++i )
        for ( unsigned int j = 0; j != 4; ++j )
            m_data [ j ][ i ] = ( i == j ? 1.0f : 0.0f );
}

// -----------------------------------------------------------------------------

inline void Matrix4 :: setTranslation ( float x, float y, float z )
{
    for ( unsigned int i = 0; i != 3; ++i )
        for ( unsigned int j = 0; j != 4; ++j )
            m_data [ i ][ j ] = ( i == j ? 1.0f : 0.0f );

    m_data [ 3 ][ 0 ] = x;
    m_data [ 3 ][ 1 ] = y;
    m_data [ 3 ][ 2 ] = z;
    m_data [ 3 ][ 3 ] = 1.0f;
}

// -----------------------------------------------------------------------------

inline void Matrix4 :: setTranslation ( const Vector4& t )
{
    setTranslation ( t [ 0 ], t [ 1 ], t [ 2 ] );
}

// -----------------------------------------------------------------------------

inline void Matrix4 :: setPerspective (
    float fovy, float aspect, float zNear, float zFar )
{
    const float tanHalfFovy = std::tan ( fovy / 2.0f );

    m_data [ 0 ][ 0 ] = 1.0f / ( aspect * tanHalfFovy );
    m_data [ 0 ][ 1 ] = 0.0f;
    m_data [ 0 ][ 2 ] = 0.0f;
    m_data [ 0 ][ 3 ] = 0.0f;
    m_data [ 1 ][ 0 ] = 0.0f;
    m_data [ 1 ][ 1 ] = 1.0f / ( tanHalfFovy );
    m_data [ 1 ][ 2 ] = 0.0f;
    m_data [ 1 ][ 3 ] = 0.0f;
    m_data [ 2 ][ 0 ] = 0.0f;
    m_data [ 2 ][ 1 ] = 0.0f;
    m_data [ 2 ][ 2 ] = zFar / ( zNear - zFar );
    m_data [ 2 ][ 3 ] = -1.0f;
    m_data [ 3 ][ 0 ] = 0.0f;
    m_data [ 3 ][ 1 ] = 0.0f;
    m_data [ 3 ][ 2 ] = -( zFar * zNear ) / (zFar - zNear);
    m_data [ 3 ][ 3 ] = 0.0f;
}

// -----------------------------------------------------------------------------

inline void Matrix4 :: setLookAt (
    const Vector4& eye,
    const Vector4& center,
    const Vector4& up )
{
	const Vector4 f = ( center - eye ).normalize();
	const Vector4 s = f.cross ( up ).normalize();
	const Vector4 u = s.cross ( f );

	m_data [ 0 ][ 0 ] = s[0];
	m_data [ 0 ][ 1 ] = u[0];
	m_data [ 0 ][ 2 ] = -f[0];
	m_data [ 0 ][ 3 ] = 0.0f;
	m_data [ 1 ][ 0 ] = s[1];
	m_data [ 1 ][ 1 ] = u[1];
	m_data [ 1 ][ 2 ] = -f[1];
	m_data [ 1 ][ 3 ] = 0.0f;
    m_data [ 2 ][ 0 ] = s[2];
	m_data [ 2 ][ 1 ] = u[2];
	m_data [ 2 ][ 2 ] =-f[2];
	m_data [ 2 ][ 3 ] = 0.0f;
	m_data [ 3 ][ 0 ] = -s.dot3 ( eye );
	m_data [ 3 ][ 1 ] = -u.dot3 ( eye );
	m_data [ 3 ][ 2 ] =  f.dot3 ( eye );
	m_data [ 3 ][ 3 ] = 1.0f;
}

// -----------------------------------------------------------------------------

inline void Matrix4 :: setRotation (
    float angle,
    const Vector4& axis )
{
	const float a = angle;
	const float c = std::cos ( a );
	const float s = std::sin ( a );

	Vector4 normAxis = axis.normalize();
	Vector4 temp = ( ( 1.0f - c ) * normAxis );

	m_data[0][0] = c + temp[0] * normAxis[0];
	m_data[0][1] = 0 + temp[0] * normAxis[1] + s * normAxis[2];
	m_data[0][2] = 0 + temp[0] * normAxis[2] - s * normAxis[1];
    m_data[0][3] = 0.0f;

	m_data[1][0] = 0 + temp[1] * normAxis[0] - s * normAxis[2];
	m_data[1][1] = c + temp[1] * normAxis[1];
	m_data[1][2] = 0 + temp[1] * normAxis[2] + s * normAxis[0];
    m_data[1][3] = 0.0f;

	m_data[2][0] = 0 + temp[2] * normAxis[0] + s * normAxis[1];
	m_data[2][1] = 0 + temp[2] * normAxis[1] - s * normAxis[0];
	m_data[2][2] = c + temp[2] * normAxis[2];
    m_data[2][3] = 0.0f;

	m_data[3][0] = 0.0f;
	m_data[3][1] = 0.0f;
	m_data[3][2] = 0.0f;
    m_data[3][3] = 1.0f;
}

// -----------------------------------------------------------------------------

inline Matrix4 Matrix4 :: transpose()
{
    Matrix4 result;

    for ( unsigned int i = 0; i != 4; ++i )
        for ( unsigned int j = 0; j != 4; ++j )
            result.m_data [ i ][ j ] = m_data [ j ][ i ];

    return result;
}

// -----------------------------------------------------------------------------

inline Matrix4 Matrix4 :: inverse()
{
	float coef00 = m_data[2][2] * m_data[3][3] - m_data[3][2] * m_data[2][3];
	float coef02 = m_data[1][2] * m_data[3][3] - m_data[3][2] * m_data[1][3];
	float coef03 = m_data[1][2] * m_data[2][3] - m_data[2][2] * m_data[1][3];

	float coef04 = m_data[2][1] * m_data[3][3] - m_data[3][1] * m_data[2][3];
	float coef06 = m_data[1][1] * m_data[3][3] - m_data[3][1] * m_data[1][3];
	float coef07 = m_data[1][1] * m_data[2][3] - m_data[2][1] * m_data[1][3];

	float coef08 = m_data[2][1] * m_data[3][2] - m_data[3][1] * m_data[2][2];
	float coef10 = m_data[1][1] * m_data[3][2] - m_data[3][1] * m_data[1][2];
	float coef11 = m_data[1][1] * m_data[2][2] - m_data[2][1] * m_data[1][2];

	float coef12 = m_data[2][0] * m_data[3][3] - m_data[3][0] * m_data[2][3];
	float coef14 = m_data[1][0] * m_data[3][3] - m_data[3][0] * m_data[1][3];
	float coef15 = m_data[1][0] * m_data[2][3] - m_data[2][0] * m_data[1][3];

	float coef16 = m_data[2][0] * m_data[3][2] - m_data[3][0] * m_data[2][2];
	float coef18 = m_data[1][0] * m_data[3][2] - m_data[3][0] * m_data[1][2];
	float coef19 = m_data[1][0] * m_data[2][2] - m_data[2][0] * m_data[1][2];

	float coef20 = m_data[2][0] * m_data[3][1] - m_data[3][0] * m_data[2][1];
	float coef22 = m_data[1][0] * m_data[3][1] - m_data[3][0] * m_data[1][1];
	float coef23 = m_data[1][0] * m_data[2][1] - m_data[2][0] * m_data[1][1];

	Vector4 fac0 ( coef00, coef00, coef02, coef03 );
	Vector4 fac1 ( coef04, coef04, coef06, coef07 );
	Vector4 fac2 ( coef08, coef08, coef10, coef11 );
	Vector4 fac3 ( coef12, coef12, coef14, coef15 );
	Vector4 fac4 ( coef16, coef16, coef18, coef19 );
	Vector4 fac5 ( coef20, coef20, coef22, coef23 );

	Vector4 vec0 ( m_data[1][0], m_data[0][0], m_data[0][0], m_data[0][0] );
	Vector4 vec1 ( m_data[1][1], m_data[0][1], m_data[0][1], m_data[0][1] );
	Vector4 vec2 ( m_data[1][2], m_data[0][2], m_data[0][2], m_data[0][2] );
	Vector4 vec3 ( m_data[1][3], m_data[0][3], m_data[0][3], m_data[0][3] );

	Vector4 inv0 ( vec1 * fac0 - vec2 * fac1 + vec3 * fac2 );
	Vector4 inv1 ( vec0 * fac0 - vec2 * fac3 + vec3 * fac4 );
	Vector4 inv2 ( vec0 * fac1 - vec1 * fac3 + vec3 * fac5 );
	Vector4 inv3 ( vec0 * fac2 - vec1 * fac4 + vec2 * fac5 );

	Vector4 signA ( +1, -1, +1, -1 );
	Vector4 signB ( -1, +1, -1, +1 );

	Matrix4 inverse ( inv0 * signA, inv1 * signB, inv2 * signA, inv3 * signB );
	Vector4 row0 = inverse.row ( 0 );

    const float dot1 = column ( 0 ).dot ( row0 );
	const float invDet = 1.0f / dot1;

	return inverse * invDet;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

inline Vector4 operator* ( const Matrix4& m, const Vector4& v )
{
    Vector4 result;

    for ( unsigned int row = 0; row != 4; ++row )
    {
        float r = 0.0f;

        for ( unsigned int col = 0; col != 4; ++col )
            r += m ( row, col ) * v [ col ];

        result [ row ] = r;
    }

    return result;
}

// -----------------------------------------------------------------------------

inline Vector4 operator* ( const Vector4& v, const Matrix4& m )
{
    Vector4 result;

    for ( unsigned int col = 0; col != 4; ++col )
    {
        float r = 0.0f;

        for ( unsigned int row = 0; row != 4; ++row )
            r += m ( row, col ) * v [ row ];

        result [ col ] = r;
    }

    return result;
}

// -----------------------------------------------------------------------------
} // namespace vppex
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

template<>
struct StructMemberTraits< vppex::Vector4 >
{
    static const bool has_member_info = true;
    static const bool is_unknown = false;
    static const bool is_matrix = false;
    static const bool is_col_major = false;
    static const unsigned int matrix_stride = sizeof ( vppex::Vector4 );
    static const unsigned int row_count = 4u;
    static const unsigned int column_count = 1u;
    static const unsigned int attrib_count = 1u;
    static const VkFormat format_code = format< float, float, float, float >::code;
    typedef float scalar_type;
    typedef TRValue< float > gscalar_type;
    typedef TRVector< gscalar_type, 4 > rvalue_type;
    typedef TLVector< gscalar_type, 4 > lvalue_type;
    typedef vppex::Vector4 data_type;
};

// -----------------------------------------------------------------------------

template<>
struct StructMemberTraits< vppex::Matrix4 >
{
    static const bool has_member_info = true;
    static const bool is_unknown = false;
    static const bool is_matrix = true;
    static const bool is_col_major = true;
    static const unsigned int matrix_stride = 4 * sizeof ( float );
    static const unsigned int row_count = 4u;
    static const unsigned int column_count = 4u;
    static const unsigned int attrib_count = 4u;
    static const VkFormat format_code = format< float, float, float, float >::code;
    typedef float scalar_type;
    typedef TRValue< float > gscalar_type;
    typedef TRMatrix< gscalar_type, 4, 4 > rvalue_type;
    typedef TLMatrix< gscalar_type, 4, 4 > lvalue_type;
    typedef vppex::Matrix4 data_type;
};

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPEXCOMMONMATH_HPP
