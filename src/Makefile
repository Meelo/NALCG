LINKER_FLAGS=-DOGRE_GUI_gtk -DOGRE_NO_FREEIMAGE -DOGRE_CONFIG_LITTLE_ENDIAN -I/usr/include/OGRE -I/usr/include/CEGUI -I/usr/include/OIS  -lCEGUIOgreRenderer -lOIS -lOgreMain -lCEGUIBase   -lboost_thread -lboost_system

CC= g++ -Wall -g -pg $(LINKER_FLAGS)

APP= ../bin/NALCG

OBJS=  main.o middleman.o logic/bishop.o logic/board.o logic/chessboard.o logic/chesspieces-meta.o logic/colour.o logic/king.o logic/knight.o logic/pawn.o logic/piece.o logic/queen.o logic/rook.o logic/square.o view/animationfactory.o view/animationmanager.o view/bishopmovementanimation.o view/bleedinganimation.o view/bufferedinputhandler.o view/camerashakeanimation.o view/checkanimation.o view/dyinganimation.o view/genericanimation.o view/kingmovementanimation.o view/knightmovementanimation.o view/movementanimation.o view/pawnmovementanimation.o view/promotionanimation.o view/queenmovementanimation.o view/rookmovementanimation.o view/view.o view/viewframelistener.o ais/ai.o ais/aiinfo.o ais/daniel/AIDaniel.o ais/daniel/MovementGenerator.o ais/daniel/Position.o ais/daniel/aiadapter.o client/network.o client/remoteplayer.o

all: $(OBJS)
	@echo -e "\033[22;32mall source files seems to be okay\033[22;39m"
	$(CC) $(OBJS) -o $(APP)
	
clean:
	rm --force $(OBJS) $(APP)

%.o: %.cpp
	$(CC) -c $< -o $@
