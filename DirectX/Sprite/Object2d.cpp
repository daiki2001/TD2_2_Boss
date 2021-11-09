#include "Object2d.h"

//�ÓI�����o�̎���
Object2d::SpriteCommon Object2d::spriteCommon;

Object2d::PipelineSet Object2d::SpriteCreateGraphicPipeline(ID3D12Device *dev)
{
	HRESULT result;
	ComPtr<ID3DBlob> vsBlob; // ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob; // �G���[�I�u�W�F�N�g

	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"SpriteVS.hlsl",  // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "vs_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&vsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char *)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"SpritePS.hlsl",   // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "ps_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&psBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char *)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"TEXCORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		}
	};

	//�O���t�B�b�N�X�p�C�v���C���̗���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	//�T���v���}�X�N
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
	//���X�^���C�U�X�e�[�g
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//�f�v�X�X�e���V���X�e�[�g
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);


	//�����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC &blenddesc = gpipeline.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	//�u�����h�ݒ�
#pragma region BLENDSET
	blenddesc.BlendEnable = true;
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;
#pragma endregion
	//#pragma region ADD
	//    blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	//    blenddesc.SrcBlend = D3D12_BLEND_ONE;
	//    blenddesc.DestBlend = D3D12_BLEND_ONE;
	//#pragma endregion
	//#pragma region SUB
	//    blenddesc.BlendOp = D3D12_BLEND_OP_SUBTRACT;
	//    blenddesc.SrcBlend = D3D12_BLEND_ONE;
	//    blenddesc.DestBlend = D3D12_BLEND_ONE;
	//#pragma endregion
	//#pragma region REVERSE
	//    blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	//    blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
	//    blenddesc.DestBlend = D3D12_BLEND_ZERO;
	//#pragma endregion
#pragma region INV
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
#pragma endregion


	//�}�`�̌o��ݒ�i�O�p�`�j
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1; // �`��Ώۂ�1��
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0�`255�w���RGBA
	gpipeline.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O


	//�f�X�N���v�^�e�[�u���̐ݒ�
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	//���[�g�p�����[�^�̐ݒ�
	CD3DX12_ROOT_PARAMETER rootparams[2];
	rootparams[0].InitAsConstantBufferView(0);
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV);

	// �e�N�X�`���T���v���[�̐ݒ�
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	//�p�C�v���C���ƃ��[�g�V�O�l�`���̃Z�b�g
	PipelineSet pipelineSet;

	// ���[�g�V�O�l�`���ݒ�
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
	ComPtr<ID3DBlob> rootSigBlob;
	//�o�[�W������������ł̃V���A���C�Y
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	//���[�g�V�O�l�`���̐���
	result = dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&pipelineSet.rootsignature));
	// �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	gpipeline.pRootSignature = pipelineSet.rootsignature.Get();
	//�W���I�Ȑݒ�i�[�x�e�X�g���s���A�������݂����A�[�x����������΍��i�j
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);	//�[�x�e�X�g���s��
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;		//��ɏ㏑��
	gpipeline.DepthStencilState.DepthEnable = false;							//�[�x�e�X�g�����Ȃ�
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;	//�[�x�l�t�H�[�}�b�g

	//�O���t�B�b�N�X�p�C�v���C���X�e�[�g�̐���
	result = dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelineSet.pipelinestate));

	return pipelineSet;
}

Object2d::Sprite Object2d::SpriteCreate(ID3D12Device *dev,int window_width, int window_height, UINT texNumber, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY)
{
	HRESULT result = S_FALSE;

	//�V�����X�v���C�g�����
	Sprite sprite{};

	//���_�f�[�^
	VertexPosUv vertices[] = {
		{{  0.0f,100.0f,0.0f},{0.0f,1.0f}},
		{{  0.0f,  0.0f,0.0f},{0.0f,0.0f}},
		{{100.0f,100.0f,0.0f},{1.0f,1.0f}},
		{{100.0f,  0.0f,0.0f},{1.0f,0.0f}}
	};

	//�e�N�X�`���ԍ��R�s�[
	sprite.texNumber = texNumber;

	//�A���J�[�|�C���g�̃R�s�[
	sprite.anchorpoint = anchorpoint;

	//���]�t���O���R�s�[
	sprite.isFlipX = isFlipX;
	sprite.isFlipY = isFlipY;


	//���_�o�b�t�@�쐬
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices)),
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&sprite.vertBuff)
	);

	//���_�o�b�t�@�փf�[�^�]��
	VertexPosUv *vertMap = nullptr;
	result = sprite.vertBuff->Map(0, nullptr, (void **)&vertMap);
	memcpy(vertMap, vertices, sizeof(vertices));
	sprite.vertBuff->Unmap(0, nullptr);

	//�w��ԍ��̉摜���ǂݍ��ݍς݂Ȃ�
	if (spriteCommon.texBuff[sprite.texNumber]) {
		//�e�N�X�`�����擾
		D3D12_RESOURCE_DESC resDesc = spriteCommon.texBuff[sprite.texNumber]->GetDesc();
		//�X�v���C�g�̑傫�����摜�̉𑜓x�ɍ��킹��
		sprite.size = { (float)resDesc.Width,(float)resDesc.Height };
		sprite.texLeftTop = { 0.0f,0.0f };
		sprite.texSize = { (float)resDesc.Width,(float)resDesc.Height };
	}

	//���_�o�b�t�@�f�[�^�]��
	SpriteTransferVertexBuffer(sprite);

	//���_�o�b�t�@�r���[�̍쐬
	sprite.vbView.BufferLocation = sprite.vertBuff->GetGPUVirtualAddress();
	sprite.vbView.SizeInBytes = sizeof(vertices);
	sprite.vbView.StrideInBytes = sizeof(vertices[0]);

	//�萔�o�b�t�@
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(Object2d::ConstBufferData)),
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&sprite.constBuff)
	);


	//�萔�o�b�t�@�Ƀf�[�^��]��
	ConstBufferData *constMap = nullptr;
	result = sprite.constBuff->Map(0, nullptr, (void **)&constMap);
	constMap->color = XMFLOAT4(1, 1, 1, 1);
	
	//�ˉe���e�s��
	constMap->mat = XMMatrixOrthographicOffCenterLH(
		0.0f, window_width, window_height, 0.0f, 0.0f, 1.0f
	);

	sprite.constBuff->Unmap(0, nullptr);
	return sprite;
}

void Object2d::SpriteCommonBeginDraw(ID3D12GraphicsCommandList *cmdList)
{
	//�p�C�v���C���X�e�[�g�̐ݒ�
	cmdList->SetPipelineState(spriteCommon.pipelineSet.pipelinestate.Get());
	//���[�g�V�O�l�`���̐ݒ�
	cmdList->SetGraphicsRootSignature(spriteCommon.pipelineSet.rootsignature.Get());
	//�v���~�e�B�u�`���ݒ�
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//�e�N�X�`���p�f�X�N���v�^�q�[�v�̐ݒ�
	ID3D12DescriptorHeap *ppHeaps[] = { spriteCommon.descHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
}

void Object2d::SpriteCommonLoadTexture(UINT texnumber, const wchar_t *filename, ID3D12Device *dev)
{
	assert(texnumber <= spriteSRVCount - 1);
	HRESULT result;

	//�e�N�X�`���̃��[�h
	TexMetadata metadata{};
	ScratchImage scratchImg{};
	result = LoadFromWICFile(
		filename,
		WIC_FLAGS_NONE,
		&metadata, scratchImg);
	const Image *img = scratchImg.GetImage(0, 0, 0);

	//���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC texresDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			metadata.format,
			metadata.width,
			(UINT)metadata.height,
			(UINT16)metadata.arraySize,
			(UINT16)metadata.mipLevels);

	// �e�N�X�`���o�b�t�@����
	result = dev->CreateCommittedResource(	//GPU���\�[�X�̐���
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc, 
		D3D12_RESOURCE_STATE_GENERIC_READ,	//�e�N�X�`���p�w��
		nullptr,
		IID_PPV_ARGS(&spriteCommon.texBuff[texnumber]));

	// �e�N�X�`���o�b�t�@�ւ̃f�[�^�]��
	result = spriteCommon.texBuff[texnumber]->WriteToSubresource(
		0,
		nullptr,	//�S�̈�փR�s�[
		img->pixels,	//���f�[�^�A�h���X
		(UINT)img->rowPitch,	//1���C���T�C�Y
		(UINT)img->slicePitch	//�S�T�C�Y
	);


	//�V�F�[�_���\�[�X�r���[�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};	//�ݒ�\����
	srvDesc.Format = metadata.format;	//RGBA
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	//2D�e�N�X�`��
	srvDesc.Texture1D.MipLevels = 1;

	//�V�F�[�_�[���\�[�X�r���[�쐬
	dev->CreateShaderResourceView(
		spriteCommon.texBuff[texnumber].Get(),	//�r���[�Ɗ֘A�t����o�b�t�@
		&srvDesc,	//�e�N�X�`���ݒ���
		CD3DX12_CPU_DESCRIPTOR_HANDLE(spriteCommon.descHeap->GetCPUDescriptorHandleForHeapStart(), texnumber,	dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV))
	);
}

void Object2d::SpriteCommonCreate(int window_width, int window_height, ID3D12Device *dev)
{
	HRESULT result = S_FALSE;

	//�X�v���C�g�p�p�C�v���C������
	spriteCommon.pipelineSet = SpriteCreateGraphicPipeline(dev);

	//�X�v���C�g�p�p�C�v���C������
	spriteCommon.matProjection = XMMatrixOrthographicOffCenterLH(
		0.0f, (float)window_width, (float)window_height, 0.0f, 0.0f, 1.0f);

	//�f�X�N���v�^�q�[�v�̐���
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NumDescriptors = spriteSRVCount;
	result = dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&spriteCommon.descHeap));
}

void Object2d::SpriteUpdate(Sprite &sprite)
{
	//���[���h�s��̍X�V
	sprite.matWorld = XMMatrixIdentity();
	//Z����]
	sprite.matWorld *= XMMatrixRotationZ(XMConvertToRadians(sprite.rotation));
	//���s�ړ�
	sprite.matWorld *= XMMatrixTranslation(sprite.position.x, sprite.position.y, sprite.position.z);
	
	//�萔�o�b�t�@�̓]��
	ConstBufferData *constMap = nullptr;
	HRESULT result = sprite.constBuff->Map(0, nullptr, (void **)&constMap);
	constMap->mat = sprite.matWorld * spriteCommon.matProjection;
	constMap->color = sprite.color;
	sprite.constBuff->Unmap(0, nullptr);
}

void Object2d::SpriteTransferVertexBuffer(const Sprite &sprite)
{
	HRESULT result = S_FALSE;
	
	//���_�f�[�^
	VertexPosUv vertices[] = {
		{{},{0.0f,1.0f}},
		{{},{0.0f,0.0f}},
		{{},{1.0f,1.0f}},
		{{},{1.0f,0.0f}}
	};
	
	enum { LB, LT, RB, RT };

	float left = (0.0f - sprite.anchorpoint.x) * sprite.size.x;
	float right = (1.0f - sprite.anchorpoint.x) * sprite.size.x;
	float top = (0.0f - sprite.anchorpoint.y) * sprite.size.y;
	float bottom = (1.0f - sprite.anchorpoint.y) * sprite.size.y;

	//���]
	if(sprite.isFlipX){
		left = -left;
		right = -right;
	}
	if(sprite.isFlipY){
		top = -top;
		bottom = -bottom;
	}

	vertices[LB].pos = { left,bottom,0.0f };
	vertices[LT].pos = { left,top,0.0f };
	vertices[RB].pos = { right,bottom,0.0f };
	vertices[RT].pos = { right,top,0.0f };

	//�w��ԍ��̉摜���ǂݍ��ݍς݂Ȃ�
	if (spriteCommon.texBuff[sprite.texNumber]) {
		//�e�N�X�`�����擾
		D3D12_RESOURCE_DESC resDesc = spriteCommon.texBuff[sprite.texNumber]->GetDesc();

		float tex_left = sprite.texLeftTop.x / resDesc.Width;
		float tex_right = (sprite.texLeftTop.x + sprite.texSize.x) / resDesc.Width;
		float tex_top = sprite.texLeftTop.y / resDesc.Height;
		float tex_bottom = (sprite.texLeftTop.y + sprite.texSize.y) / resDesc.Height;

		vertices[LB].uv = { tex_left, tex_bottom };
		vertices[LT].uv = { tex_left, tex_top };
		vertices[RB].uv = { tex_right, tex_bottom };
		vertices[RT].uv = { tex_right, tex_top };
	}

	//���_�o�b�t�@�ւ̃f�[�^�]��
	VertexPosUv *vertMap = nullptr;
	result = sprite.vertBuff->Map(0, nullptr, (void **)&vertMap);
	memcpy(vertMap, vertices, sizeof(vertices));
	sprite.vertBuff->Unmap(0, nullptr);
}

void Object2d::SpriteDraw(const Sprite &sprite, ID3D12GraphicsCommandList *cmdList,  ID3D12Device *dev) {	
	if (sprite.isInvisible) {
		return;
	}


	//���_�o�b�t�@���Z�b�g
	cmdList->IASetVertexBuffers(0, 1, &sprite.vbView);
	//�萔�o�b�t�@���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, sprite.constBuff->GetGPUVirtualAddress());
	//�V�F�[�_���\�[�X�r���[���Z�b�g
	cmdList->SetGraphicsRootDescriptorTable(
		1, CD3DX12_GPU_DESCRIPTOR_HANDLE(
			spriteCommon.descHeap->GetGPUDescriptorHandleForHeapStart(),
			sprite.texNumber,
			dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
	//�|���S���̕`��i4���_�̎l�p�`�j
	cmdList->DrawInstanced(4, 1, 0, 0);
}