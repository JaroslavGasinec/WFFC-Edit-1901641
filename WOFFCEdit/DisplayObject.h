#pragma once
#include "pch.h"

using DirectX::SimpleMath::Vector3;
using DirectX::IEffect;

class DisplayObject
{
public:
	DisplayObject();
	~DisplayObject();

	std::shared_ptr<DirectX::Model>						m_model;							//main Mesh
	ID3D11ShaderResourceView *							m_texture_diffuse;					//diffuse texture

	int             m_ID;
	Vector3			m_position;
	Vector3			m_orientation;
	Vector3			m_scale;
	bool									m_render;
	bool									m_wireframe;

	int		m_light_type;
	float	m_light_diffuse_r, m_light_diffuse_g, m_light_diffuse_b;
	float	m_light_specular_r, m_light_specular_g, m_light_specular_b;
	float	m_light_spot_cutoff;
	float	m_light_constant;
	float	m_light_linear;
	float	m_light_quadratic;

	// marking for commits
	bool m_pendingCommit = false;

	//selection marking (optimized instead of constantly looking up the selection array)
	bool m_isSelected = false;

	// possible changes
	void Move(const Vector3& worldOffset);
	void Rotate(const Vector3& offsetRotation);
	void Scale(const Vector3& offsetScale);
	void Reset();

	void MarkSelected();
	void UnmarkSelected();
	bool IsSelected();
};

