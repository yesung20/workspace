package main.borad;

import java.util.ArrayList;
import java.util.List;

public class BulletinBoard {
    List<Article> articleList = new ArrayList<>();
    Article article;
    int nextNum=0;

    public List<Article> showAll(){
        return articleList;
    }

    public void create(String userId, String title, String content){
        article = new Article();
        article.setUserId(userId);
        article.setTitle(title);
        article.setContent(content);
        article.setArticleNum(++nextNum);
        articleList.add(article);
        System.out.println("[ finish ]");
    }
    public Article read(int articleNum){
        return articleList.get(articleNum-1);
    }

    public void update(int articleNum, String title, String content){
        for(int i=0; i<articleList.size(); i++){
            if(articleNum == articleList.get(i).getArticleNum()){
                article.setTitle(title);
                article.setContent(content);
                break;
            }
        }
        System.out.println("[ finish ]");
    }

    public void delete(int articleNum){
        for(int i=0; i<articleList.size(); i++){
            if(articleNum == articleList.get(i).getArticleNum()){
                articleList.remove(i);
                break;
            }
        }
        System.out.println("[ finish ]");
    }

}
