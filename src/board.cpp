#include "../header/board.hpp"
#include "../header/camera.hpp"
#include "../class/ShaderManager.hpp"

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 
#include <iostream>

extern Camera* camera;

Board::Board(std::string const &path) : Model(path){
	// board position;
	pos.x = 0.0f;
	pos.y = 0.0f; //-1.75f; 
	pos.z = 0.0f; //-1.0f;

	// tile size;
	tileSize.x = 0.50f;
	tileSize.y = 0.50f;
	edge.x = 0.25f;
	edge.y = 0.25f;

	for(int k = 0; k  < 8; k++){
		for (int y = 0; y < 8; y++){
			tiles[k][y] = nullptr;
		}
	}

	Piece pawn = Piece("../asset/chessTemp/Pawn.obj", "Pawn", true);
	Piece rook = Piece("../asset/chessTemp/Rook.obj", "Rook", true);
	Piece bishop = Piece("../asset/chessTemp/Bishop.obj", "Bishop", true);

	rook.setColor(true);
	tiles[0][0] = new Piece(rook);
	rook.setColor(false);
	tiles[0][7] = new Piece(rook);
	tiles[2][0] = new Piece(bishop);
	pawn.setColor(false);
	tiles[0][2] = new Piece(pawn);

/*	Piece knight = Piece("../asset/chessTemp/Knight.obj", "Horse", true);
	Piece bishop = Piece("../asset/chessTemp/Bishop.obj", "Bishop", true);
	Piece queen = Piece("../asset/chessTemp/Queen.obj", "Queen", true);
	Piece king = Piece("../asset/chessTemp/King.obj", "King", true);

	for (int i = 0; i < 8; ++i){
		pawn.setColor(true);
		tiles[i][1] = new Piece(pawn);
		pawn.setColor(false);
		tiles[i][6] = new Piece(pawn);	
	}

	tiles[0][0] = new Piece(rook);
	tiles[1][0] = new Piece(knight);
	tiles[2][0] = new Piece(bishop);
	tiles[3][0] = new Piece(queen);
	tiles[4][0] = new Piece(king);
	tiles[5][0] = new Piece(bishop);
	tiles[6][0] = new Piece(knight);
	tiles[7][0] = new Piece(rook);
	

	rook.setColor(false);
	knight.setColor(false);
	bishop.setColor(false);
	queen.setColor(false);
	king.setColor(false);
	tiles[0][7] = new Piece(rook);
	tiles[1][7] = new Piece(knight);
	tiles[2][7] = new Piece(bishop);
	tiles[3][7] = new Piece(queen);
	tiles[4][7] = new Piece(king);
	tiles[5][7] = new Piece(bishop);
	tiles[6][7] = new Piece(knight);
	tiles[7][7] = new Piece(rook);
	*/
	float offset = 1.15f;//3.14f / 6.0f;
	
	float x, y, z;
	for(int i = 0; i < 8; i++){
		for(int j = 0; j < 8; j++){
			if(tiles[i][j] != nullptr){
				x = pos.x + (-tileSize.x*4 + edge.x + (tileSize.x * i));//+ (-4 * offset); // + (j * offset + (offset / 2));
				y = 0.0f + pos.y;//-1.15f;
				z = pos.y + (-tileSize.y*4 + edge.y + (tileSize.y * j));//+ (-3 * offset);//+ (i * offset);

				std::cout << "i:" << i << ", j:" << j << '\n';
				std::cout << "x: " << x << ", y:" << y << ", z:" << z << '\n'; 
				tiles[i][j]->place(glm::vec3(x, y, z));
				//	+x --- right, -x --- left
				//	+y --- up,   -y --- down
				//  +z ---- front, -z --- back
			}
		}
	}

	extern ShaderManager* shaderManager;
	shaderProgram = shaderManager->getShader(std::vector<std::pair<GLenum, std::string>>{
		{GL_VERTEX_SHADER, "../shader/vertex.vert"},
		{GL_FRAGMENT_SHADER, "../shader/fragment.frag"},
	});

};


void Board::movePiece(int indexI, int indexJ, int destinationI, int destinationJ, float dt){	
	
	std::vector<std::pair<int, int>> allAvailableMoves = moveToIndex(indexI, indexJ);
	glm::vec3 newPosition, position;
		//TODO JUMP CURVE

	for(auto v : allAvailableMoves){
		if(v.first == destinationI && v.second == destinationJ){
			
			glm::vec3 destination = glm::vec3(
												(pos.x + (-tileSize.x * 4 + edge.x + (tileSize.x * destinationI))),
												 pos.y, 
												(pos.y + (-tileSize.y * 4 + edge.y + (tileSize.y * destinationJ)))
												);

			if(tiles[indexI][indexJ]->getName() == "Horse"){
				std::cout << "im here!\n";
				position = glm::vec3(
												(pos.x + (-tileSize.x * 4 + edge.x + (tileSize.x * indexI))),
												 pos.y, 
												(pos.y + (-tileSize.y * 4 + edge.y + (tileSize.y * indexJ)))
												);
				glm::vec3 midpoint = glm::vec3((position.x + destination.x)/2, 1.0f, (position.z + destination.z)/2);

				newPosition = jumpCurve(position, midpoint, destination, dt);

				

				tiles[indexI][indexJ]->place(newPosition);

				if(dt == 1){
					tiles[destinationI][destinationJ] = tiles[indexI][indexJ];
					tiles[indexI][indexJ] = nullptr;
				}
			
			} else {
				position = glm::vec3(
										(pos.x + (-tileSize.x * 4 + edge.x + (tileSize.x * indexI))),
										 pos.y, 
										(pos.y + (-tileSize.y * 4 + edge.y + (tileSize.y * indexJ)))
										);

				newPosition = lerp(position, destination, dt);
				tiles[indexI][indexJ]->place(newPosition);
				if(dt == 1){
					tiles[destinationI][destinationJ] = tiles[indexI][indexJ];
					tiles[indexI][indexJ] = nullptr;
				}
			}
				
		}
	}
/*	if(t == 1){
		tiles[destinationI][destinationJ] = tiles[indexI][indexJ];
		tiles[indexI][indexJ] = nullptr;	
	
	}*/
	/*if((indexJ+f) == destinationJ){

	
	}
	else {
		std::cout << "piece " << indexI << ", " << indexJ << " cant move to desired destination " << destinationI << ", " << destinationJ << "\n";
	}*/
}



auto Board::moveToIndex(int indexI, int indexJ) -> std::vector<std::pair<int, int>> {
	std::vector<std::pair<int, int>> allAvailableMoves;
	std::vector<std::string> movesList;

	if(tiles[indexI][indexJ]){
		movesList = tiles[indexI][indexJ]->getMoves();
	
		for(auto i : movesList){
			if(i == "F"){
				if(tiles[indexI][indexJ]->pieceColor()){ 
					if(tiles[indexI][indexJ + 1] == nullptr && tiles[indexI][indexJ]->getName() == "Pawn"){ 
						allAvailableMoves.push_back(std::pair<int, int>(indexI, indexJ + 1));
					}
					if(tiles[indexI][indexJ + 1] == nullptr && tiles[indexI][indexJ]->getName() == "King"){
						allAvailableMoves.push_back(std::pair<int, int>(indexI, indexJ + 1));
					} else if (!tiles[indexI][indexJ + 1]->pieceColor()){
						allAvailableMoves.push_back(std::pair<int, int>(indexI, indexJ + 1));
					}
				} else {
					if(tiles[indexI][indexJ - 1] == nullptr && tiles[indexI][indexJ]->getName() == "Pawn"){ 
						allAvailableMoves.push_back(std::pair<int, int>(indexI, indexJ - 1));
					}
					if(tiles[indexI][indexJ - 1] == nullptr && tiles[indexI][indexJ]->getName() == "King"){
						allAvailableMoves.push_back(std::pair<int, int>(indexI, indexJ - 1));
					} else if (tiles[indexI][indexJ + 1]->pieceColor()){
						allAvailableMoves.push_back(std::pair<int, int>(indexI, indexJ - 1));
					}
				}
			} else if (i == "B"){
				if(tiles[indexI][indexJ]->pieceColor()) {
					if(tiles[indexI][indexI - 1] == nullptr){
						allAvailableMoves.push_back(std::pair<int, int>(indexI, indexJ - 1));
					} else if (!tiles[indexI][indexJ - 1]->pieceColor()){
						allAvailableMoves.push_back(std::pair<int, int>(indexI, indexJ - 1));
					}
				} else {
					if(tiles[indexI][indexJ + 1] == nullptr){
						allAvailableMoves.push_back(std::pair<int, int>(indexI, indexJ + 1));
					} else if (tiles[indexI][indexJ + 1]->pieceColor()){
						allAvailableMoves.push_back(std::pair<int, int>(indexI, indexJ + 1));
					}
				}
			} else if (i == "L"){
				if(tiles[indexI][indexJ]->pieceColor()){
					if(tiles[indexI + 1][indexJ] == nullptr){
						allAvailableMoves.push_back(std::pair<int, int>(indexI + 1, indexJ));
					} else if (!tiles[indexI + 1][indexJ]->pieceColor()){
						allAvailableMoves.push_back(std::pair<int, int>(indexI + 1, indexJ));
					}
				} else {
					if(tiles[indexI - 1][indexJ] == nullptr){
						allAvailableMoves.push_back(std::pair<int, int>(indexI - 1, indexJ));	
					} else if(tiles[indexI - 1][indexJ]->pieceColor()){
						allAvailableMoves.push_back(std::pair<int, int>(indexI - 1, indexJ));
					}
				}
			} else if (i == "R"){
				if(tiles[indexI][indexJ]->pieceColor()){
					if(tiles[indexI - 1][indexJ] == nullptr){
						allAvailableMoves.push_back(std::pair<int, int>(indexI - 1, indexJ));
					} else if(!tiles[indexI - 1][indexJ]->pieceColor()){
						allAvailableMoves.push_back(std::pair<int, int>(indexI - 1, indexJ));
					}
				} else {
					if(tiles[indexI + 1][indexJ] == nullptr){
						allAvailableMoves.push_back(std::pair<int, int>(indexI + 1, indexJ));
					} else if (tiles[indexI + 1][indexJ]->pieceColor()){
						allAvailableMoves.push_back(std::pair<int, int>(indexI + 1, indexJ));
					}
				
				}
			} else if (i == "F*"){
				if(tiles[indexI][indexJ]->pieceColor()){
					for(int j = indexJ + 1; j < 8; j++){
						if(tiles[indexI][j] == nullptr){
							allAvailableMoves.push_back(std::pair<int, int>(indexI, j));
						} else if(!tiles[indexI][j]->pieceColor()){
							allAvailableMoves.push_back(std::pair<int, int>(indexI, j));
							break;
						} else if(tiles[indexI][j]->pieceColor()){
							break;
						}
					}
				} else { 
					for(int j = indexJ - 1; j >= 0; j--){
						if(tiles[indexI][j] == nullptr){
							allAvailableMoves.push_back(std::pair<int, int>(indexI, j));
						} else if(tiles[indexI][j]->pieceColor()){
							allAvailableMoves.push_back(std::pair<int, int>(indexI, j));
							break;
						} else if(!tiles[indexI][j]->pieceColor()){
							break;
						}
					}
				}
			} else if (i == "B*"){
				if(tiles[indexI][indexJ]->pieceColor()) {
					for(int j = indexJ - 1; j >= 0; j--){
						if(tiles[indexI][j] == nullptr){
							allAvailableMoves.push_back(std::pair<int, int>(indexI, j));
						} else if(!tiles[indexI][j]->pieceColor()){
							allAvailableMoves.push_back(std::pair<int, int>(indexI, j));
							break;
						} else if(tiles[indexI][j]->pieceColor()){
							break;
						}
					}
				} else  {
					for(int j = indexJ + 1; j < 8; j++){
						if(tiles[indexI][j] == nullptr){
							allAvailableMoves.push_back(std::pair<int, int>(indexI, j));
						} else if(tiles[indexI][j]->pieceColor()){
							allAvailableMoves.push_back(std::pair<int, int>(indexI, j));
							break;
						} else if(!tiles[indexI][j]->pieceColor()){
							break;
						}

					}
				}
			} else if (i == "L*") {
				if(tiles[indexI][indexJ]->pieceColor()){
					for(int i = indexI + 1; i < 8; i++){
						if(tiles[i][indexJ] == nullptr){
							allAvailableMoves.push_back(std::pair<int, int>(i, indexJ));
						} else if(!tiles[i][indexJ]->pieceColor()){
							allAvailableMoves.push_back(std::pair<int, int>(i, indexJ));
							break;
						} else if(tiles[i][indexJ]->pieceColor()){
							break;
						}
					}
				} else { 
					for(int i = indexI - 1; i >= 0; i--){
						if(tiles[i][indexJ] == nullptr){
							allAvailableMoves.push_back(std::pair<int, int>(i, indexJ));
						} else if(tiles[i][indexJ]->pieceColor()){
							allAvailableMoves.push_back(std::pair<int, int>(i, indexJ));
							break;
						} else if(!tiles[i][indexJ]->pieceColor()){
							break;
						}
					}
				}
			} else if (i == "R*"){
				if(tiles[indexI][indexJ]->pieceColor()){
					for(int i = indexI - 1; i >= 0; i--){
						if(tiles[i][indexJ] == nullptr){
							allAvailableMoves.push_back(std::pair<int, int>(i, indexJ));
						} else if(!tiles[i][indexJ]->pieceColor()){
							allAvailableMoves.push_back(std::pair<int, int>(i, indexJ));
							break;
						} else if(tiles[i][indexJ]->pieceColor()){
							break;
						}
					}
				} else { 
					for(int i = indexI + 1; i < 8; i++){
						if(tiles[i][indexJ] == nullptr){
							allAvailableMoves.push_back(std::pair<int, int>(i, indexJ));
						} else if(tiles[i][indexJ]->pieceColor()){
							allAvailableMoves.push_back(std::pair<int, int>(i, indexJ));
							break;
						} else if(!tiles[i][indexJ]->pieceColor()){
							break;
						}
				}	}
			} else if (i == "(FR)*"){
				if(tiles[indexI][indexJ]->pieceColor()) {
					for(int i = indexI - 1, j = indexJ + 1; i >= 0 || j > 8; i--, j++){
						allAvailableMoves.push_back(std::pair<int, int>(i, j));
					}
				} else { 
					for(int i = indexI + 1, j = indexJ - 1; i < 8 || j >= 0; i++, j--){
						allAvailableMoves.push_back(std::pair<int, int>(i, j));
					}
				}	
			}/* else if (i == "(FL)*"){
				if(tiles[indexI][indexJ]->pieceColor())
					for(int i = indexI, j = indexJ; i < 8 || j < 8; i++, j++){
						allAvailableMoves.push_back(std::pair<int, int>(i, j));
					}
				else 
					for(int i = indexI, j = indexJ; i >= 0 || j >= 0; i--, j--){
						allAvailableMoves.push_back(std::pair<int, int>(i, j));
				}
			} else if (i == "(BR)*"){
				if(tiles[indexI][indexJ]->pieceColor())
					for(int i = indexI, j = indexJ; i >= 0 || j >= 0; i--, j--){
						allAvailableMoves.push_back(std::pair<int, int>(i, j));
					}
				else 
					for(int i = indexI, j = indexJ; i < 8 || j < 8; i++, j++){
						allAvailableMoves.push_back(std::pair<int, int>(i, j));
				}
			} else if (i == "(BL)*"){
				if(tiles[indexI][indexJ]->pieceColor())
					for(int i = indexI, j = indexJ; i < 8 || j >= 0; i++, j--){
						allAvailableMoves.push_back(std::pair<int, int>(i, j));
					}
				else 
					for(int i = indexI, j = indexJ; i >= 0 || j > 8; i--, j++){
						allAvailableMoves.push_back(std::pair<int, int>(i, j));
				}
			} else if (i == "FFR"){
				if(tiles[indexI][indexJ]->pieceColor())
					if (indexI - 1 >= 0 && indexJ + 2 < 8)
						allAvailableMoves.push_back(std::pair<int, int>(indexI - 1, indexJ + 2));
				else 
					if (indexI + 1 < 8 && indexJ - 2 >= 0){
						allAvailableMoves.push_back(std::pair<int, int>(indexI + 1 , indexJ - 2));
					}
			} else if (i == "FFL"){
				if(tiles[indexI][indexJ]->pieceColor())
					if (indexI + 1 < 8 && indexJ + 2 < 8)
						allAvailableMoves.push_back(std::pair<int, int>(indexI + 1, indexJ + 2));
				else 
					if (indexI - 1 >= 0 && indexJ - 2 >= 0){
						allAvailableMoves.push_back(std::pair<int, int>(indexI - 1 , indexJ - 2));
					}
			} else if (i == "BBR"){
				if(tiles[indexI][indexJ]->pieceColor()){
					if (indexI - 1 >= 0 && indexJ - 2 >= 0){
						allAvailableMoves.push_back(std::pair<int, int>(indexI - 1, indexJ - 2));
					}
				}
				else {
					if (indexI + 1 < 8 && indexJ + 2 < 8){
						allAvailableMoves.push_back(std::pair<int, int>(indexI + 1 , indexJ + 2));
					}
				}
			}else if (i == "BBL"){
				if(tiles[indexI][indexJ]->pieceColor())
					if (indexI + 1 < 8 && indexJ - 2 >= 0)
						allAvailableMoves.push_back(std::pair<int, int>(indexI + 1, indexJ - 2));
				else 
					if (indexI - 1 >= 0 && indexJ + 2 < 8){
						allAvailableMoves.push_back(std::pair<int, int>(indexI - 1 , indexJ + 2));
					}
			} else if (i == "LLF"){
				if(tiles[indexI][indexJ]->pieceColor())
					if (indexI + 2 < 8 && indexJ + 1 < 8)
						allAvailableMoves.push_back(std::pair<int, int>(indexI + 2, indexJ + 1));
				else 
					if (indexI - 2 >= 0 && indexJ - 1 >= 0){
						allAvailableMoves.push_back(std::pair<int, int>(indexI - 2 , indexJ - 1));
					}
			} else if (i == "LLB"){
				if(tiles[indexI][indexJ]->pieceColor())
					if (indexI + 2 < 8 && indexJ - 1 >= 0)
						allAvailableMoves.push_back(std::pair<int, int>(indexI + 2, indexJ - 1));
				else 
					if (indexI - 2 >= 0 && indexJ + 1 < 8){
						allAvailableMoves.push_back(std::pair<int, int>(indexI - 2 , indexJ + 1));
					}
			} else if (i == "RRF"){
				if(tiles[indexI][indexJ]->pieceColor())
					if (indexI - 2 >= 0 && indexJ + 1 < 8)
						allAvailableMoves.push_back(std::pair<int, int>(indexI - 2, indexJ + 1));
				else 
					if (indexI + 2 < 8 && indexJ - 1 >= 0){
						allAvailableMoves.push_back(std::pair<int, int>(indexI + 2 , indexJ - 1));
					}
			} else if (i == "RRB"){
				if(tiles[indexI][indexJ]->pieceColor())
					if (indexI - 2 >= 0 && indexJ - 1 >= 0)
						allAvailableMoves.push_back(std::pair<int, int>(indexI - 2, indexJ - 1));
				else 
					if (indexI + 2 < 8 && indexJ + 1 < 8){
						allAvailableMoves.push_back(std::pair<int, int>(indexI + 2 , indexJ + 1));
					}
			} else if (i == "(FR)"){
				if(tiles[indexI][indexJ]->pieceColor())
					if (indexI - 1 >= 0 && indexJ + 1 < 8)
						allAvailableMoves.push_back(std::pair<int, int>(indexI - 1, indexJ + 1));
				else 
					if (indexI + 1 < 8 && indexJ - 1 >= 0){
						allAvailableMoves.push_back(std::pair<int, int>(indexI + 1 , indexJ - 1));
					}
			} else if (i == "(FL)"){
				if(tiles[indexI][indexJ]->pieceColor())
					if (indexI + 1 < 8 && indexJ + 1 < 8)
						allAvailableMoves.push_back(std::pair<int, int>(indexI + 1, indexJ + 1));
				else 
					if (indexI - 1 >= 0 && indexJ - 1 >= 0){
						allAvailableMoves.push_back(std::pair<int, int>(indexI - 1 , indexJ - 1));
					}
			} else if (i == "(BL)"){
				if(tiles[indexI][indexJ]->pieceColor())
					if (indexI + 1 < 8 && indexJ - 1 >= 0)
						allAvailableMoves.push_back(std::pair<int, int>(indexI + 1, indexJ - 1));
				else 
					if (indexI - 1 >= 0 && indexJ + 1 < 8){
						allAvailableMoves.push_back(std::pair<int, int>(indexI - 1 , indexJ + 1));
					}
			} else if (i == "(BR)"){
				if(tiles[indexI][indexJ]->pieceColor())
					if (indexI - 1 >= 0 && indexJ - 1 >= 0)
						allAvailableMoves.push_back(std::pair<int, int>(indexI - 1, indexJ - 1));
				else 
					if (indexI + 1 < 8 && indexJ + 1 < 8){
						allAvailableMoves.push_back(std::pair<int, int>(indexI + 1 , indexJ + 1));
					}
			}*/
		}	
	} else {
		//std::cout << "Piece " << indexI << ", " << indexJ << " doesnt exist\n";	
	} 
	return allAvailableMoves;
}


void Board::draw(){
	shaderProgram->bind();

	static float time = 0.0;
	time += .01;

	//glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = camera->getViewMatrix(); //glm::lookAt(glm::vec3(0, 0, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	//glm::mat4 view = glm::lookAt(glm::vec3(0, 5, 0), glm::vec3(0, 4, 0), glm::vec3(0, 0, -1));
	glm::mat4 projection = camera->getPerspectiveMatrix();//glm::perspective(3.14f / 3.0f, (GLfloat)1024 / (GLfloat)768, 0.1f, -10.0f);

	// Rotate light for effect
	lightSourcePosition.x = sin(glfwGetTime()) * 3.0f;
	lightSourcePosition.z = cos(glfwGetTime()) * 2.0f;
	lightSourcePosition.y = 5.0 + cos(glfwGetTime()) * 1.0f;

	std::map<std::string, GLuint> uniforms = shaderProgram->getUniform(	std::map<std::string, GLchar*>({
		{"viewID", "view"},
		{"projectionID", "projection"},
		{"modelID", "model"},
		{"normalMatrixID", "normalMatrix"},
		{"lightSourcePositionID","lightSourcePosition"},
		{"camPosID", "CamPos"},
		{"attenuationAID", "attenuationA"},
		{"attenuationBID", "attenuationB"},
		{"attenuationCID", "attenuationC"},
		{"lightColorID", "lightColor"}
	}));
	// TODO: get light information from camera / view.
	glUniform1f(uniforms["attenuationAID"], attenuationA);
	glUniform1f(uniforms["attenuationBID"], attenuationB);
	glUniform1f(uniforms["attenuationCID"], attenuationC);
	glUniform3fv(uniforms["lightColorID"], 1, value_ptr(lightColor));
	glUniform3fv(uniforms["lightSourcePositionID"], 1, value_ptr(lightSourcePosition));
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);

	glUniform3fv(uniforms["camPosID"], 1, value_ptr(position));												//glm::mat4 model = glm::rotate(glm::mat4(), time, glm::vec3(0, 1, 0));

	glUniformMatrix4fv(uniforms["viewID"], 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(uniforms["projectionID"], 1, GL_FALSE, glm::value_ptr(projection));
	glm::mat4 modelm;
	modelm = glm::translate(modelm, pos); // translate it down so it's at the center of the scene
	//modelm = glm::translate(modelm, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
	modelm = glm::scale(modelm, glm::vec3(0.4f, 0.4f, 0.4f));	// it's a bit too big for our scene, so scale it down
	//modelm = glm::rotate(modelm, time, glm::vec3(0, 1, 0));	
													//ourShader.setMat4("model", model);
	glUniformMatrix4fv(uniforms["modelID"], 1, GL_FALSE, glm::value_ptr(modelm));
	
	glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(view*modelm)));
	glUniformMatrix3fv(uniforms["normalMatrixID"], 1, GL_FALSE, glm::value_ptr(normalMatrix));
	
	Model::Draw(*shaderProgram); 
	 //printf("hello\t");

	shaderProgram->unbind();

	// TODO: draw pieces

	for(int i = 0; i < 8; i++){
		for(int j = 0; j < 8; j++){
			if(tiles[i][j] != nullptr){
				tiles[i][j]->draw();
			}
		}
	}
}

auto Board::lerp(glm::vec3 a, glm::vec3 b, float dt) -> glm::vec3 {
	glm::vec3 point;

	point.x = a.x * (1 - dt) + b.x * dt;
	point.y = a.y * (1 - dt) + b.y * dt;
	point.z = a.z * (1 - dt) + b.z * dt;

	return point;

}

auto Board::getPoint(float p0, float p1, float dt) -> float{
	float diff = p1 - p0;

	return p0 + (diff * dt);
}

auto Board::jumpCurve(glm::vec3 a, glm::vec3 b, glm::vec3 c, float dt) -> glm::vec3 {
	glm::vec3 point1, point2, result;

	point1.x = getPoint(a.x, b.x, dt);
	point1.y = getPoint(a.y, b.y, dt);
	point1.z = getPoint(a.z, b.z, dt);

	point2.x = getPoint(b.x, c.x, dt);
	point2.y = getPoint(b.y, c.y, dt);
	point2.z = getPoint(b.z, c.z, dt);

	result.x = getPoint(point1.x, point2.x, dt);
	result.y = getPoint(point1.y, point2.y, dt);
	result.z = getPoint(point1.z, point2.z, dt);

  	return result;
}

glm::vec2 Board::getTileSize()
{
	return this->tileSize;
}

glm::vec3 Board::getPosition()
{
	return this->pos;
}

glm::vec2 Board::getEdge()
{
	return this->edge;
}
