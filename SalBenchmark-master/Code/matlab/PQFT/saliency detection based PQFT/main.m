clc

clear

seqs.img_list = dir('F:\git\paper1st\SalBenchmark-master\Data\DataSet3\ImgsPQFT\*.jpg');

N = length(seqs.img_list);

for i=1:N
   img = imread(['F:\git\paper1st\SalBenchmark-master\Data\DataSet3\ImgsPQFT\', seqs.img_list(i).name]);
   out = getImage(img);
   [~, frameName] = fileparts(seqs.img_list(i).name);
   imwrite(out,  ['F:\git\paper1st\SalBenchmark-master\Data\DataSet3\Saliency\' frameName '_PQFT' '.png']);
end

%imwrite(out{i},  ['F:\git\paper1st\SalBenchmark-master\Data\DataSet3\Saliency\' num2str(i) '.png']);


% function varargout = main(varargin)
% % MAIN M-file for main.fig
% %      MAIN, by itself, creates a new MAIN or raises the existing
% %      singleton*.
% %
% %      H = MAIN returns the handle to a new MAIN or the handle to
% %      the existing singleton*.
% %
% %      MAIN('CALLBACK',hObject,eventData,handles,...) calls the local
% %      function named CALLBACK in MAIN.M with the given input arguments.
% %
% %      MAIN('Property','Value',...) creates a new MAIN or raises the
% %      existing singleton*.  Starting from the left, property value pairs are
% %      applied to the GUI before main_OpeningFcn gets called.  An
% %      unrecognized property name or invalid value makes property application
% %      stop.  All inputs are passed to main_OpeningFcn via varargin.
% %
% %      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
% %      instance to run (singleton)".
% %
% % See also: GUIDE, GUIDATA, GUIHANDLES
% 
% % Edit the above text to modify the response to help main
% 
% % Last Modified by GUIDE v2.5 17-May-2013 14:38:01
% 
% % Begin initialization code - DO NOT EDIT
% gui_Singleton = 1;
% gui_State = struct('gui_Name',       mfilename, ...
%                    'gui_Singleton',  gui_Singleton, ...
%                    'gui_OpeningFcn', @main_OpeningFcn, ...
%                    'gui_OutputFcn',  @main_OutputFcn, ...
%                    'gui_LayoutFcn',  [] , ...
%                    'gui_Callback',   []);
% if nargin && ischar(varargin{1})
%     gui_State.gui_Callback = str2func(varargin{1});
% end
% 
% if nargout
%     [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
% else
%     gui_mainfcn(gui_State, varargin{:});
% end
% % End initialization code - DO NOT EDIT
% 
% 
% % --- Executes just before main is made visible.
% function main_OpeningFcn(hObject, eventdata, handles, varargin)
% % This function has no output args, see OutputFcn.
% % hObject    handle to figure
% % eventdata  reserved - to be defined in a future version of MATLAB
% % handles    structure with handles and user data (see GUIDATA)
% % varargin   command line arguments to main (see VARARGIN)
% 
% % Choose default command line output for main
% handles.output = hObject;
% 
% % Update handles structure
% guidata(hObject, handles);
% 
% % UIWAIT makes main wait for user response (see UIRESUME)
% % uiwait(handles.figure1);
% 
% 
% % --- Outputs from this function are returned to the command line.
% function varargout = main_OutputFcn(hObject, eventdata, handles) 
% % varargout  cell array for returning output args (see VARARGOUT);
% % hObject    handle to figure
% % eventdata  reserved - to be defined in a future version of MATLAB
% % handles    structure with handles and user data (see GUIDATA)
% 
% % Get default command line output from handles structure
% varargout{1} = handles.output;
% 
% 
% % --------------------------------------------------------------------
% function Untitled_1_Callback(hObject, eventdata, handles)
% % hObject    handle to Untitled_1 (see GCBO)
% % eventdata  reserved - to be defined in a future version of MATLAB
% % handles    structure with handles and user data (see GUIDATA)
% 
% 
% % --------------------------------------------------------------------
% function Untitled_5_Callback(hObject, eventdata, handles)
% % hObject    handle to Untitled_5 (see GCBO)
% % eventdata  reserved - to be defined in a future version of MATLAB
% % handles    structure with handles and user data (see GUIDATA)
% 
% 
% % --------------------------------------------------------------------
% function about_Callback(hObject, eventdata, handles)
% % hObject    handle to about (see GCBO)
% % eventdata  reserved - to be defined in a future version of MATLAB
% % handles    structure with handles and user data (see GUIDATA)
% 
% 
% % --------------------------------------------------------------------
% function open_Callback(hObject, eventdata, handles)
% % hObject    handle to open (see GCBO)
% % eventdata  reserved - to be defined in a future version of MATLAB
% % handles    structure with handles and user data (see GUIDATA)
% [filename pathname]=uigetfile({'*.jpg';'*.bmp';'*.tif';'*.*'},'Please select a color image');
% if isequal(filename,0)||isequal(pathname,0)
%     errorlg('没有选中文件','出错');
%     return;
% else
%     file=[pathname,filename];
%     global S         %保存文件的初始路径    
%     S=file;
%     handles.img=imread(file);
%     set(handles.axes1,'HandleVisibility','ON');
%     axes(handles.axes1);
%     imshow(handles.img);
%     set(handles.axes1,'HandleVisibility','OFF');
%     guidata(hObject, handles);
% end
% 
% % --------------------------------------------------------------------
% function save_Callback(hObject, eventdata, handles)
% % hObject    handle to save (see GCBO)
% % eventdata  reserved - to be defined in a future version of MATLAB
% % handles    structure with handles and user data (see GUIDATA)
% [sfilename,sfilepath]=uiputfile({'*.jpg';'*.bmp';'*.tif';'*.*'},'Save a saliency image','untitled.jpg');
% if ~isequal([sfilename,sfilepath],[0,0])
%     sfilefullname=[sfilepath,sfilename];
%     imwrite(handles.ssM,sfilefullname);
% else
%     msgbox('你按了取消键','保存失败');
% end
% % --------------------------------------------------------------------
% function quit_Callback(hObject, eventdata, handles)
% % hObject    handle to quit (see GCBO)
% % eventdata  reserved - to be defined in a future version of MATLAB
% % handles    structure with handles and user data (see GUIDATA)
% clc;
% close all;
% close(gcf);
% clear;
% 
% % --- Executes on button press in pushbutton1.
% function pushbutton1_Callback(hObject, eventdata, handles)
% % hObject    handle to pushbutton1 (see GCBO)
% % eventdata  reserved - to be defined in a future version of MATLAB
% % handles    structure with handles and user data (see GUIDATA)
% handles.ssM=getImage(handles.img);
% axes(handles.axes2);
% imshow(handles.ssM);
% guidata(hObject, handles);
% 
% % --- Executes on button press in pushbutton2.
% function pushbutton2_Callback(hObject, eventdata, handles)
% % hObject    handle to pushbutton2 (see GCBO)
% % eventdata  reserved - to be defined in a future version of MATLAB
% % handles    structure with handles and user data (see GUIDATA)
% expectation=mean(handles.ssM(:));
% handles.imbw=im2bw(handles.ssM,3*expectation);
% axes(handles.axes3);
% imshow(handles.imbw);
% guidata(hObject, handles);
% 
% % --- Executes on button press in pushbutton3.
% function pushbutton3_Callback(hObject, eventdata, handles)
% % hObject    handle to pushbutton3 (see GCBO)
% % eventdata  reserved - to be defined in a future version of MATLAB
% % handles    structure with handles and user data (see GUIDATA)
% handles.ob=detection(handles.imbw,handles.img);
% axes(handles.axes4);
% imshow(handles.ob);
% guidata(hObject, handles);
% 
% % --- Executes during object creation, after setting all properties.
% function axes1_CreateFcn(hObject, eventdata, handles)
% % hObject    handle to axes1 (see GCBO)
% % eventdata  reserved - to be defined in a future version of MATLAB
% % handles    empty - handles not created until after all CreateFcns called
% set(hObject,'xTick',[]);
% set(hObject,'ytick',[]);
% set(hObject,'box','on');
% 
% % Hint: place code in OpeningFcn to populate axes1
% 
% 
% % --- Executes during object creation, after setting all properties.
% function axes2_CreateFcn(hObject, eventdata, handles)
% % hObject    handle to axes2 (see GCBO)
% % eventdata  reserved - to be defined in a future version of MATLAB
% % handles    empty - handles not created until after all CreateFcns called
% set(hObject,'xTick',[]);
% set(hObject,'ytick',[]);
% set(hObject,'box','on');
% 
% % Hint: place code in OpeningFcn to populate axes2
% 
% 
% % --- Executes during object creation, after setting all properties.
% function axes3_CreateFcn(hObject, eventdata, handles)
% % hObject    handle to axes3 (see GCBO)
% % eventdata  reserved - to be defined in a future version of MATLAB
% % handles    empty - handles not created until after all CreateFcns called
% set(hObject,'xTick',[]);
% set(hObject,'ytick',[]);
% set(hObject,'box','on');
% 
% % Hint: place code in OpeningFcn to populate axes3
% 
% 
% % --- Executes during object creation, after setting all properties.
% function axes4_CreateFcn(hObject, eventdata, handles)
% % hObject    handle to axes4 (see GCBO)
% % eventdata  reserved - to be defined in a future version of MATLAB
% % handles    empty - handles not created until after all CreateFcns called
% set(hObject,'xTick',[]);
% set(hObject,'ytick',[]);
% set(hObject,'box','on');
% 
% % Hint: place code in OpeningFcn to populate axes4
