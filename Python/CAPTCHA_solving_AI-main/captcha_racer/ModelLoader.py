import torch
import torch.nn as nn
import torchvision
from torchvision import transforms
import torch.nn.functional as F

import numpy as np
import cv2
import random
import gc


gc.collect()
torch.cuda.empty_cache()

IMAGE_SIZE = 256
MAX_LEN = 10
RANDOM_SEED = random.randint(0, 2**32-1)


def random_seed():
    torch.manual_seed(RANDOM_SEED)
    torch.cuda.manual_seed(RANDOM_SEED)
    torch.cuda.manual_seed_all(RANDOM_SEED)
    torch.backends.cudnn.deterministic = True
    torch.backends.cudnn.benchmark = False
    np.random.seed(RANDOM_SEED)
    random.seed(RANDOM_SEED)

    print('Random Seed : {0}'.format(RANDOM_SEED))
    

special_char_list = ["<pad>"] # ["<unk>", "<pad>"]
num_list = ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9']
upper_alphabet_list = ['A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z']
lower_alphabet_list = ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z']

string_list = special_char_list + num_list + upper_alphabet_list + lower_alphabet_list
CHAR_NUM = len(string_list)

token_dictionary = {i : string_list[i] for i in range(len(string_list))}
reversed_token_dictionary = {v: k for k, v in token_dictionary.items()}

def torch_tensor_to_plt(img):
    img = img.detach().numpy()[0]
    img = np.transpose(img, (1, 2, 0))
    return img 



transformer = transforms.Compose([transforms.ToTensor(),
                                  torchvision.transforms.Resize((IMAGE_SIZE,IMAGE_SIZE)),
                                  transforms.Normalize((0.5, 0.5, 0.5), (0.5, 0.5, 0.5)),
                                 ])

class LACC(nn.Module):
    def __init__(self):
        super().__init__()
        self.encoder = torchvision.models.efficientnet_v2_m().features
        self.converter = nn.parameter.Parameter(torch.ones(64, CHAR_NUM))        

        self.silu = nn.SiLU()
        self.linear1 = nn.Linear(1280, 512)
        self.linear2 = nn.Linear(512, 64)
        self.linear3 = nn.Linear(64, MAX_LEN)
        

    def forward(self, x):
        # print(x.shape)
        feature = self.encoder(x)
        #print(feature.shape)
        feature = torch.flatten(feature, start_dim=2)
        #print(feature.shape)
        feature = torch.matmul(feature, self.converter)
        
        y = feature.transpose(-1, -2)
        y = self.linear1(y)
        y = self.silu(y)
        y = self.linear2(y)
        y = self.silu(y)
        y = self.linear3(y)
        
        return y

class ImageLoader:
    transformer = transforms.Compose([transforms.ToTensor(),
                                  torchvision.transforms.Resize((IMAGE_SIZE,IMAGE_SIZE)),
                                  transforms.Normalize((0.5, 0.5, 0.5), (0.5, 0.5, 0.5)),
                                 ])
    
    def __init__(self):
        self.transformer = ImageLoader.transformer
        
    def transform(self, image):
        if self.transformer!=None:
            return self.transformer(image)
        else :
            return image

    def loadImage(self, filename):
        
        Y = []
        for char in list(filename.split("/")[-1].split(".")[0]):
            if(char == "_"):
                break
            Y.append(reversed_token_dictionary[char])
            
        if len(Y) < MAX_LEN:
            Y += [reversed_token_dictionary["<pad>"]]*(MAX_LEN-len(Y))
        
        img = cv2.imread(filename)
        try:
            sketch_image = cv2.cvtColor(img[:,:256,:], cv2.COLOR_BGR2RGB)
        except:
            print(filename)
        X = self.transform(sketch_image)
        # print(X.shape)
        
        Y_tensor_list = []
        for y_ind in Y:
            y_tensor = torch.zeros(CHAR_NUM)
            y_tensor[y_ind] = 1
            Y_tensor_list.append(y_tensor.unsqueeze(0))

        return X, torch.tensor(Y)
    



class ModelLoader:
    

    def __init__(self, checkpointPath):
        random_seed()

        use_cuda = torch.cuda.is_available()
        self.device = torch.device("cuda" if use_cuda else "cpu")
        self.cpu_device = torch.device("cpu")

        self.model = LACC().to(self.device)
        # self.optimizer = Lion(self.model.parameters(), lr=lr, weight_decay=1e-2)

        checkpoint = torch.load(checkpointPath, map_location=self.device, weights_only=True)
        self.model.load_state_dict(checkpoint["model_state_dict"])
        # self.optimizer.load_state_dict(checkpoint['optimizer_state_dict'])

        self.image_loader = ImageLoader()




    def predictImage(self, filename, batch=0):
        def replaceSpecialToken(text):
            text = text.replace('<pad>','□')
            text = text.replace('<unk>','?')
            return text

        x, target = self.image_loader.loadImage(filename)
    
        x, target = x.to(self.device), target.to(self.device)
        self.model.eval()
        
        x = x.unsqueeze(0)
        predict = self.model(x)
        predict = F.log_softmax(predict, dim=-2)
        predict = torch.argmax(predict, dim=-2)
        
        predict_text = ""
        for token in predict[0].to(self.cpu_device).tolist():
            if token == 0:
                break
            predict_text += str(token_dictionary[token])
            
        # target_text = ""
        # for token in target[0].to(self.cpu_device).tolist():
        #     target_text += str(token_dictionary[token])
            
        predict_text = replaceSpecialToken(predict_text)
        # target_text = replaceSpecialToken(target_text)
            

        
        return predict_text #, target text


